/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "EmitterMacros.h"
#include "Timer.h"

int main(void)
{
  TIM_Delay_Init();
  EMIT_Init();

  for(;;) {
    IR_FRONT_ON;
    TIM_Delay_Micro(60);
    IR_FRONT_OFF;

    IR_FLEFT_ON;
    TIM_Delay_Micro(60);
    IR_FLEFT_OFF;

    IR_FRIGHT_ON;
    TIM_Delay_Micro(60);
    IR_FRIGHT_OFF;

    IR_SIDES_ON;
    TIM_Delay_Micro(60);
    IR_SIDES_OFF;

    TIM_Delay_Milli(1);
  }
}
