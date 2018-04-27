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
#include "stm32f4xx_gpio.h"
#include "PWMDriver.hpp"
#include "Timer.h"

int main(void)
{
  TIM_Delay_Init();
  MotorDriver motorLeft(GPIOB, GPIO_Pin_9, TIM10, GPIOB, GPIO_Pin_8, TIM11);

  for(;;) {
    for(uint8_t i=0; i<50; i++) {
      motorLeft.driveForward(i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    for(int16_t i=50; i>=0; i--) {
      motorLeft.driveForward((uint8_t)i);
      TIM_Delay_Milli(100);
    }
    motorLeft.stallMotors();
    TIM_Delay_Micro(10);
    for(uint8_t i=0; i<50; i++) {
      motorLeft.driveBackward(i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    for(int16_t i=50; i>=0; i--) {
      motorLeft.driveBackward((uint8_t)i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    motorLeft.stallMotors();
    TIM_Delay_Milli(3000);
  }
}
