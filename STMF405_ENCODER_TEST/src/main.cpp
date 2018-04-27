/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Kenmei
  * @brief   Basic encoder tracking using the type def directly. It could be done
  *          using the global structs included. Take a look at the .h file to see
  *          the elements.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "DualEncoder.h"
#include "Timer.h"
#include "HardwareSerial.h"

int main(void)
{
  // initialize peripherals
  TIM_Delay_Init();
  HUSART_begin(USART3, 9600);
  ENC_Init();
  ENC_SystTickInit();

  for(;;) {
    HUSART_writeString(USART3, (char*)"Left: ");
    HUSART_writeInt(USART3, TIM5->CNT);
    HUSART_writeString(USART3, (char*)"\tRIght: ");
    HUSART_writeInt(USART3, TIM2->CNT);
    HUSART_writeString(USART3, (char*)"\n");
    TIM_Delay_Milli(50);
  }
}
