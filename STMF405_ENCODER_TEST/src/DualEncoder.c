#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "DualEncoder.h"

// few global variables for tracking
volatile struct SpeedStruct LRSpeedStruct;
volatile struct DistStruct LRDistTotal;
volatile struct TempStruct LRTempStruct;

void ENC_Read() {
  if(LRDistTotal.fwdTotal > 2000000000)
    ENC_Reset();
  LRTempStruct.oldLeftEncoder = LRTempStruct.leftEncoder;
  LRTempStruct.leftEncoder = TIM_GetCounter(_ENCL_TIMER);
  LRTempStruct.oldRightEncoder = LRTempStruct.rightEncoder;
  LRTempStruct.rightEncoder = -TIM_GetCounter(_ENCR_TIMER);
  LRSpeedStruct.leftCount = LRTempStruct.leftEncoder - LRTempStruct.oldLeftEncoder;
  LRSpeedStruct.rightCount = LRTempStruct.rightEncoder - LRTempStruct.oldRightEncoder;
  LRSpeedStruct.fwdCount = LRSpeedStruct.leftCount + LRSpeedStruct.rightCount;
  LRSpeedStruct.rotCount = -(LRSpeedStruct.leftCount - LRSpeedStruct.rightCount);
  LRDistTotal.fwdTotal += LRSpeedStruct.fwdCount;
  LRDistTotal.rotTotal += LRSpeedStruct.rotCount;
  LRDistTotal.leftTotal += LRSpeedStruct.leftCount;
  LRDistTotal.rightTotal += LRSpeedStruct.rightCount;
}

void ENC_Reset() {
  __disable_irq();
  LRTempStruct.oldLeftEncoder = 0;
  LRTempStruct.oldRightEncoder = 0;
  LRDistTotal.leftTotal = 0;
  LRDistTotal.rightTotal = 0;
  LRDistTotal.fwdTotal = 0;
  LRDistTotal.rotTotal = 0;
  TIM_SetCounter(_ENCL_TIMER, 0);
  TIM_SetCounter(_ENCR_TIMER, 0);
  ENC_Read();
  __enable_irq();
}

void ENC_Init() {
  // turn on encoder clocks and timer clocks
  _ENCL_TIMER_CLK_EN;
  _ENCR_TIMER_CLK_EN;
  RCC_AHB1PeriphClockCmd(_ENCLA_GPIO_CLK | _ENCLB_GPIO_CLK |
                         _ENCRA_GPIO_CLK | _ENCRB_GPIO_CLK, ENABLE);

  // Initialize Pins
  GPIO_InitTypeDef GPIOInitStruct;
  GPIO_StructInit(&GPIOInitStruct);
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIOInitStruct.GPIO_Pin = _ENCLA_PIN;
  GPIO_Init(_ENCLA_GPIO_PORT, &GPIOInitStruct);
  GPIOInitStruct.GPIO_Pin = _ENCLB_PIN;
  GPIO_Init(_ENCLB_GPIO_PORT, &GPIOInitStruct);
  GPIOInitStruct.GPIO_Pin = _ENCRA_PIN;
  GPIO_Init(_ENCRA_GPIO_PORT, &GPIOInitStruct);
  GPIOInitStruct.GPIO_Pin = _ENCRB_PIN;
  GPIO_Init(_ENCRB_GPIO_PORT, &GPIOInitStruct);

  // Connect to timers
  GPIO_PinAFConfig(_ENCLA_GPIO_PORT, _ENCLA_SOURCE, _ENCLA_AF);
  GPIO_PinAFConfig(_ENCLB_GPIO_PORT, _ENCLB_SOURCE, _ENCLB_AF);
  GPIO_PinAFConfig(_ENCRA_GPIO_PORT, _ENCRA_SOURCE, _ENCRA_AF);
  GPIO_PinAFConfig(_ENCRB_GPIO_PORT, _ENCRB_SOURCE, _ENCRB_AF);

  /**
   * Sets up timers based on a specified configuration
   * --> TIM_EncoderMode_TI12 ===> Read both encoder channels
   * --> TIM_EncoderMode_TI1  ===> Read only first channel
   * --> TIM_EncoderMode_TI2  ===> Read only second channel
   *
   * The second part of the function simply sets the type of signal to
   * tick from. (i.e. tick only on rising side for TIM_ICPolarity_Rising)
   */
  TIM_EncoderInterfaceConfig(_ENCL_TIMER, TIM_EncoderMode_TI12,
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload(_ENCL_TIMER, 0xffff);
  TIM_EncoderInterfaceConfig(_ENCR_TIMER, TIM_EncoderMode_TI12,
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload(_ENCR_TIMER, 0xffff);

  // Turn on Timers
  TIM_Cmd(_ENCL_TIMER, ENABLE);
  TIM_Cmd(_ENCR_TIMER, ENABLE);
  ENC_Reset();
}

void ENC_SystTickInit() {
  // call sys-tick interrupt every ms
  SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(void) {
  ENC_Read();
}
