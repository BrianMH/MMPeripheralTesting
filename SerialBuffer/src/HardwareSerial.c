#include <HardwareSerial.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include <misc.h>

/************* SERIAL INTERFACE FUNCTIONS ****************/
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
   // TODO: FINISH IMPLEMENTATION FOR DIFFERENT USART

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
   if(twrite < 0) {
      neg = true;
      twrite *= -1;
   }

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

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

bool HUSART_writeFloat(USART_TypeDef* usart, double twrite) {
	   RingBuffer* txbuf = _HUSART_RET_BUF(usart, __TX);
	   bool neg = false;
	   int8_t ind = __PRECISION+1;

	   // Find mantissa portion
	   int nonman = (int)twrite;
	   int mantissa = (int)((twrite - nonman)*ipow(10, __PRECISION));

	   // Do int conversion
	   char buf[11+__PRECISION+1] = {0};
	   buf[__PRECISION] = '.';

	   // check if char is negative
	   if(nonman < 0) {
	      neg = true;
	      nonman *= -1;
	      mantissa *= -1;
	   }

	   // Perform a simple conversion of main number
	   for(; ind < 10+__PRECISION; ind++) {
	      buf[ind] = (char)( 48 + (nonman % 10));
	      nonman /= 10;
	      if(nonman == 0)
	         break;   // if nonman is now zero, number conversion is done
	   }

	   // Now add decimal and convert mantissa to certain position
	   for(int ind2 = 0; ind2 < __PRECISION; ind2++) {
		  buf[ind2] = (char)( 48 + (mantissa % 10));
		  mantissa /= 10;
		  if(mantissa == 0) {
			  if(neg)
				  buf[++ind] = '-';
			  break;   // if mantissa is now zero, number conversion is done
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

/************* RING BUFFER FUNCTIONS ****************/
// More of an insurance that values are not garbage
void RingBuffer_init(RingBuffer* buf) {
   buf->_head = 0;
   buf->_tail = 0;
   for(unsigned short ind = 0; ind < _DEFAULT_RING_SIZE; ind++) {
      buf->_buffer[ind] = 0;
   }
}

bool RingBuffer_isEmpty(RingBuffer* buf) {
   return buf->_head==buf->_tail;
}

bool RingBuffer_isFull(RingBuffer* buf) {
   return (((buf->_head)+1) % _DEFAULT_RING_SIZE)==buf->_tail;
}

bool RingBuffer_queue(RingBuffer* buf, uint8_t toAdd) {
   if(RingBuffer_isFull(buf)) // buffer is full; do nothing
      return false;

   buf->_buffer[buf->_head] = toAdd;
   if(++(buf->_head) >= _DEFAULT_RING_SIZE)
      buf->_head = 0;
   return true;
}

uint8_t RingBuffer_dequeue(RingBuffer* buf) {
   if(RingBuffer_isEmpty(buf)) // buffer is empty; nothing to do
      return 0;

   uint8_t ret = buf->_buffer[buf->_tail];
   if(++(buf->_tail) >= _DEFAULT_RING_SIZE)
      buf->_tail = 0;
   return ret;
}
