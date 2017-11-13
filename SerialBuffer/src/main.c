/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include <stm32f4xx.h>
#include <HardwareSerial.h>

int main(void)
{
   // Initialize Serial
   HUSART_begin(USART2, 9600);
   HUSART_writeChar(USART2, '-');
   HUSART_writeString(USART2, "Testing the Serial interface!\n");
   HUSART_writeInt(USART2, 1234);

	while(1) {
//	   static char c = '\0';
//	   if(HUSART_available(USART2)) {
//	      c = HUSART_read(USART2);
//	      HUSART_writeString(USART2, "Found a char!\t");
//	      HUSART_writeChar(USART2, c);
//	      HUSART_writeChar(USART2, '\n');
//	   }
	}
}
