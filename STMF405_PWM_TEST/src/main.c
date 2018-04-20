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
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "Timer.h"
			
void setupMotorPWM();
void initializePins();

int main(void)
{
  TIM_Delay_Init();
  setupMotorPWM();
  initializePins();

  for(;;) {
    // ramp up pwm speed
    for(int i=0; i<100; i++) {
      TIM11->CCR1 = i;
      TIM_Delay_Micro(10);
    }
    TIM_Delay_Micro(2000);
    // ramp down
    for(int i=99; i>=0; i--) {
      TIM11->CCR1 = i;
      TIM_Delay_Micro(10);
    }
    TIM_Delay_Micro(2000);
  }
}

void initializePins() {
  // pull PB8 down and use PB9 through TIM10-CH1 as PWM enforcer
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIOInitStruct;
  GPIOInitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIOInitStruct.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(GPIOB, &GPIOInitStruct);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM11);

  GPIOInitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;
  GPIO_Init(GPIOB, &GPIOInitStruct);
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

void setupMotorPWM() {
  // Enable the clock for the PWM signal
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);

  // Initialize timer via struct
  TIM_TimeBaseInitTypeDef TIMTimeBaseStruct;
  TIM_TimeBaseStructInit (&TIMTimeBaseStruct);
  // Notice that you calculate the the timer frequency
  // and then scale based on the TIM bus frequency:
  // new_frequency = APB1/2_CLOCK / PRESCALER
  TIMTimeBaseStruct.TIM_Prescaler = 168;
  // TIM_Period determines the PWM frequency
  // For example, 1kHz can be attained through...
  // ENSURE TIM_Period LESS THAN MAX TIMER VALUE (2^16 for most)!
  // TIM_Period = (timer_clock / PWM_frequency) - 1
  TIMTimeBaseStruct.TIM_Period = 100; // 1kHz rate
  TIMTimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMTimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM11, &TIMTimeBaseStruct);
  TIM_Cmd(TIM11, ENABLE);

  TIM_OCInitTypeDef TIMOCStruct;
  // MODE 2 -> Clear on compare match
  // MODE 1 -> Set on compare match
  TIMOCStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIMOCStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIMOCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIMOCStruct.TIM_Pulse = 1;
  TIM_OC1Init(TIM11, &TIMOCStruct);
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
  TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRConfig(TIM11, &TIM_BDTRInitStruct);
  TIM_CCPreloadControl(TIM11, ENABLE);
  TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM11, ENABLE);
}
