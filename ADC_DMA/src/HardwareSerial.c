#include <RingBuffer.h>
#include <HardwareSerial.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

void HUSART_begin(USART_TypeDef* usart, uint32_t baud) {
   USART_InitTypeDef USART_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructureRx;
   GPIO_InitTypeDef GPIO_InitStructureTx;

   // Set up GPIO structs
   GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructureRx.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructureRx.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructureRx.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructureTx.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructureTx.GPIO_OType = GPIO_OType_PP;

   // Set up USART struct
   USART_InitStructure.USART_BaudRate = baud;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//   // Ensure the the usart peripheral is off before setting it up
//   USART_Cmd(usart, DISABLE);
//   USART_DeInit(usart);

   // Varying usart entries have varying implementations
   if(usart == USART2) {      // USART2 ON PA2/PA3
      // Initialize corresponding clocks
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

      //Initialize the pins with structs
      GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_3;
      GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_2;
      GPIO_Init(GPIOA, &GPIO_InitStructureRx);
      GPIO_Init(GPIOA, &GPIO_InitStructureTx);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

      // Initialize USART
      USART_Init(USART2, &USART_InitStructure);

      // Activate interrupt system (Set up optional precedence at your leisure)
      USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
      NVIC_EnableIRQ(USART2_IRQn);

      // Initializes the structs meant for this application
      RingBuffer_init(&rx2Buf);
      RingBuffer_init(&tx2Buf);
   }

   USART_Cmd(usart, ENABLE);
}

uint8_t HUSART_read(USART_TypeDef* usart) {
   RingBuffer* rxbuf = _HUSART_RET_BUF(usart, __RX);

   return RingBuffer_dequeue(rxbuf);
}

RingBuffer* _HUSART_RET_BUF(USART_TypeDef* usart, RTX_Type type) {
   if(usart == USART2)
      if(type == __RX)
         return &rx2Buf;
      else
         return &tx2Buf;
   else
      return (void*)0;
}

bool HUSART_writeChar(USART_TypeDef* usart, char twrite) {
   RingBuffer* txbuf = _HUSART_RET_BUF(usart, __TX);

   // buffer is full. transmission failed
   if(RingBuffer_isFull(txbuf))
      return false;

   // attempt transmission
   RingBuffer_queue(txbuf, (uint8_t)twrite);
   USART_ITConfig(usart, USART_IT_TXE, ENABLE);
   return true;
}

bool HUSART_writeString(USART_TypeDef* usart, char* twrite) {
   RingBuffer* txbuf = _HUSART_RET_BUF(usart, __TX);
   char curr = *twrite;
   uint8_t index = 1;

   while(curr != '\0') {
      // buffer is full. transmission failed
      if(RingBuffer_isFull(txbuf))
         return false;

      // check for escape characters
      if(curr == '\\') {
         volatile char futchar = *(twrite+(++index));
         if(futchar == 'n')
            RingBuffer_queue(txbuf, (uint8_t)'\n');
         else if(futchar == 't')
            RingBuffer_queue(txbuf, (uint8_t)'\t');
         else if(futchar == '\\')
            RingBuffer_queue(txbuf, (uint8_t)'\\');
         else
            index--; // No possible escape char, just use the char as is
      }

      // attempt transmission
      RingBuffer_queue(txbuf, (uint8_t)curr);
      curr = *(twrite + index++);  // grab next char and increment
   }

   USART_ITConfig(usart, USART_IT_TXE, ENABLE);
   return true;
}

bool HUSART_writeInt(USART_TypeDef* usart, int twrite) {
   RingBuffer* txbuf = _HUSART_RET_BUF(usart, __TX);
   bool neg = false;
   int8_t ind = 0;

   // Do int conversion
   char buf[11] = {0};

   // check if char is negative
   if(twrite < 0)
      neg = true;

   // Perform a simple conversion
   for(; ind < 10; ind++) {
      buf[ind] = (char)( 48 + (twrite % 10));
      twrite /= 10;
      if(twrite == 0) {
         if(neg)
            buf[++ind] = '-';
         break;   // if twrite is now zero, number conversion is done
      }
   }

   // now perform the transfer
   while(ind >= 0) {
      // buffer is full. transmission failed
      if(RingBuffer_isFull(txbuf))
         return false;

      // attempt transmission
      RingBuffer_queue(txbuf, (uint8_t)buf[ind]);
      ind--;
   }

   USART_ITConfig(usart, USART_IT_TXE, ENABLE);
   return true;
}


bool HUSART_available(USART_TypeDef* usart) {
   RingBuffer* rxbuf = _HUSART_RET_BUF(usart, __RX);

   return !RingBuffer_isEmpty(rxbuf);
}

void HUSART_flush(USART_TypeDef* usart){
   RingBuffer* rxbuf = _HUSART_RET_BUF(usart, __RX);
   RingBuffer* txbuf = _HUSART_RET_BUF(usart, __TX);

   // Resets buffers by reinitializing them
   RingBuffer_init(rxbuf);
   RingBuffer_init(txbuf);
}

void USART2_IRQHandler(void) {
   if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
      // Place new char in rx buffer
      RingBuffer_queue(&rx2Buf, USART_ReceiveData(USART2));
   if(USART_GetITStatus(USART2, USART_IT_TXE) == SET) {
      // Send more characters from buffer
      if(RingBuffer_isEmpty(&tx2Buf))
         USART_ITConfig(USART2, USART_IT_TXE, DISABLE);  // empty. done sending
      else
         USART_SendData(USART2, RingBuffer_dequeue(&tx2Buf)); // still have more to send
   }
}
