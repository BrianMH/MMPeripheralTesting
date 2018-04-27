/*
 * MotorPWM.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: brian
 */
#include "PWMDriver.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

PWMDriver::PWMDriver(GPIO_TypeDef* pinGroup, uint16_t gpioPin, TIM_TypeDef* pwmTimer,
                     uint32_t frequency, FreqAdjust* adjuster) : pwmTimer(pwmTimer) {
  // turn on necessary clocks (note that all GPIO initializations are found in AHB1 group
  if(pinGroup == GPIOA)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  else if(pinGroup == GPIOB)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  else if(pinGroup == GPIOC)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  else if(pinGroup == GPIOD)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  else if(pinGroup == GPIOH)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

  // turning on the timer clock needs a bit more work since it may be from wither APB1 or APB2
  uint8_t GPIO_AF;
  if(pwmTimer == TIM1) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    GPIO_AF = GPIO_AF_TIM1;
  } else if(pwmTimer == TIM2) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    GPIO_AF = GPIO_AF_TIM2;
  } else if(pwmTimer == TIM3) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    GPIO_AF = GPIO_AF_TIM3;
  } else if(pwmTimer == TIM4) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    GPIO_AF = GPIO_AF_TIM4;
  } else if(pwmTimer == TIM5) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    GPIO_AF = GPIO_AF_TIM5;
//  } else if(pwmTimer == TIM6) {
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//    GPIO_AF = GPIO_AF_TIM6;
//  } else if(pwmTimer == TIM7) {
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//    GPIO_AF = GPIO_AF_TIM7;
  } else if(pwmTimer == TIM8) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    GPIO_AF = GPIO_AF_TIM8;
  } else if(pwmTimer == TIM9) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    GPIO_AF = GPIO_AF_TIM9;
  } else if(pwmTimer == TIM10) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    GPIO_AF = GPIO_AF_TIM10;
  } else if(pwmTimer == TIM11) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
    GPIO_AF = GPIO_AF_TIM11;
  } else if(pwmTimer == TIM12) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
    GPIO_AF = GPIO_AF_TIM12;
  } else if(pwmTimer == TIM13) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
    GPIO_AF = GPIO_AF_TIM13;
  }

  // find pinSource from original pin
  uint16_t pinSource;
  switch(gpioPin) {
  case(GPIO_Pin_0):pinSource = GPIO_PinSource0; break;
  case(GPIO_Pin_1):pinSource = GPIO_PinSource1; break;
  case(GPIO_Pin_2):pinSource = GPIO_PinSource2; break;
  case(GPIO_Pin_3):pinSource = GPIO_PinSource3; break;
  case(GPIO_Pin_4):pinSource = GPIO_PinSource4; break;
  case(GPIO_Pin_5):pinSource = GPIO_PinSource5; break;
  case(GPIO_Pin_6):pinSource = GPIO_PinSource6; break;
  case(GPIO_Pin_7):pinSource = GPIO_PinSource7; break;
  case(GPIO_Pin_8):pinSource = GPIO_PinSource8; break;
  case(GPIO_Pin_9):pinSource = GPIO_PinSource9; break;
  case(GPIO_Pin_10):pinSource = GPIO_PinSource10; break;
  case(GPIO_Pin_11):pinSource = GPIO_PinSource11; break;
  case(GPIO_Pin_12):pinSource = GPIO_PinSource12; break;
  case(GPIO_Pin_13):pinSource = GPIO_PinSource13; break;
  case(GPIO_Pin_14):pinSource = GPIO_PinSource14; break;
  case(GPIO_Pin_15):pinSource = GPIO_PinSource15; break;
  }

  // initialize GPIO Pin
  GPIO_InitTypeDef GPIOInitStruct;
  GPIOInitStruct.GPIO_Pin = gpioPin;
  GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIOInitStruct.GPIO_Speed = GPIO_High_Speed;
  GPIO_Init(pinGroup, &GPIOInitStruct);
  GPIO_PinAFConfig(pinGroup, pinSource, GPIO_AF);

  // setup TIM for PWM usage
  TIM_TimeBaseInitTypeDef TIMTimeBaseStruct;
  TIM_TimeBaseStructInit (&TIMTimeBaseStruct);

  if(adjuster == nullptr) {
    // Notice that you calculate the the timer frequency
    // and then scale based on the TIM bus frequency:
    // new_frequency = APB1/2_CLOCK / PRESCALER
    TIMTimeBaseStruct.TIM_Prescaler = 168;     // 100kHz base frequency
    // TIM_Period determines the PWM frequency
    // For example, 1kHz can be attained through...
    // ENSURE TIM_Period LESS THAN MAX TIMER VALUE (2^16 for most)!
    // TIM_Period = (timer_clock / PWM_frequency) - 1
    TIMTimeBaseStruct.TIM_Period = 100000/frequency; // careful about integer arithmetic! Range near
                                                     // 10000 unstable! Use alternative method for
                                                     // other values!
  } else {
    // Adjust based on user acknowledged settings
    TIMTimeBaseStruct.TIM_Prescaler = adjuster->clockPrescaler;
    TIMTimeBaseStruct.TIM_Period = adjuster->timerPeriod;
  }
  TIMTimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMTimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(pwmTimer, &TIMTimeBaseStruct);
  TIM_Cmd(pwmTimer, ENABLE);

  // setup timer
  TIM_OCInitTypeDef TIMOCStruct;
  // MODE 2 -> Clear on compare match
  // MODE 1 -> Set on compare match
  TIMOCStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIMOCStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIMOCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIMOCStruct.TIM_Pulse = 0;
  TIM_OC1Init(pwmTimer, &TIMOCStruct);
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
  TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRConfig(pwmTimer, &TIM_BDTRInitStruct);
  TIM_CCPreloadControl(pwmTimer, ENABLE);
  TIM_OC1PreloadConfig(pwmTimer, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(pwmTimer, ENABLE);
}

void PWMDriver::changeDutyCycle(const uint8_t dutyCycle) {
  uint32_t temp = ((pwmTimer->ARR)*(uint32_t)dutyCycle)/100 - 1;
  if(temp >= (pwmTimer->ARR))
    temp = 0;
  pwmTimer->CCR1 = temp;
}

void PWMDriver::changeFrequency(const uint32_t freq) {
  // change auto-reload value (period)
  pwmTimer->ARR = 100000/freq;
}

void PWMDriver::changeFrequency(const FreqAdjust& adjustment) {
  // change desired prescaler and period values
  pwmTimer->PSC = adjustment.clockPrescaler;
  pwmTimer->ARR = adjustment.timerPeriod;
}

MotorDriver::MotorDriver(GPIO_TypeDef* pinGroupForward, uint16_t gpioPinForward,
                         TIM_TypeDef* pwmTimerForward,
                         GPIO_TypeDef* pinGroupBackward, uint32_t gpioPinBackward,
                         TIM_TypeDef* pwmTimerBackward) :
                       forward(PWMDriver(pinGroupForward, gpioPinForward, pwmTimerForward, DEFAULT_FREQUENCY)),
                       backward(PWMDriver(pinGroupBackward, gpioPinBackward, pwmTimerBackward, DEFAULT_FREQUENCY)){
  // nothing needed in here
}

void MotorDriver::driveBackward(const uint8_t dutyCycle) {
  forward.changeDutyCycle(0);
  backward.changeDutyCycle(dutyCycle);
}

void MotorDriver::driveForward(const uint8_t dutyCycle) {
  backward.changeDutyCycle(0);
  forward.changeDutyCycle(dutyCycle);
}

void MotorDriver::stallMotors(void) {
  backward.changeDutyCycle(0);
  forward.changeDutyCycle(0);
}
