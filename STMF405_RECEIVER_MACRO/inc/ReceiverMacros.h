/**
  ******************************************************************************
  * @file    ReceiverMacros.h
  * @author  Kenmei
  * @brief   Macros that can be used in order to read ADC values from the
  *          IR receivers. [Look towards the bottom for the macros]
  *          Possible future iteration will include a DMA read option.
  ******************************************************************************
*/
#ifndef RECEIVERMACROS_H_
#define RECEIVERMACROS_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "Timer.h"

#ifdef __cplusplus
  extern "C" {
#endif

// Initializes all the receievers with following arrangement:
//    REC_L    ------->    PB1(9)
//    REC_BL   ------->    PC2 (12)
//    REC_FL   ------->    PC1 (11)
//    REC F    ------->    PC0 (10)
//    REC_FR   ------->    PC4 (14)
//    REC_BR   ------->    PB0 (8)
//    REC_R    ------->    PC3(13)
// Collectively, these form ADC1 Channels 8-14 so DMA transfer
// is possible...
void REC_Init() {
  // enable clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |
                         RCC_APB2Periph_ADC2 |
                         RCC_APB2Periph_ADC3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |
                         RCC_AHB1Periph_GPIOC, ENABLE);

  // set up pin for ADC use
  GPIO_InitTypeDef GPIOInitStruct;
  GPIOInitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                            GPIO_Pin_2 | GPIO_Pin_3 |
                            GPIO_Pin_4;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIOInitStruct);

  GPIOInitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIOInitStruct);

  // set up ADC
  ADC_DeInit();   // makes sure ADC is off beforehand

  ADC_CommonInitTypeDef ADCComStruct;
  ADCComStruct.ADC_Mode = ADC_Mode_Independent;
  ADCComStruct.ADC_Prescaler = ADC_Prescaler_Div2;
  ADCComStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADCComStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
  ADC_CommonInit(&ADCComStruct);

  ADC_InitTypeDef ADCInitStruct;
  ADCInitStruct.ADC_Resolution = ADC_Resolution_12b;  // 4096 bits of resolution
  ADCInitStruct.ADC_ScanConvMode = DISABLE;
  ADCInitStruct.ADC_ContinuousConvMode = DISABLE;  // single conversion mode needed for 1 pin
  ADCInitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // external interrupt not needed
  ADCInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;  // unused
  ADCInitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // least->sig bit alignment
  ADCInitStruct.ADC_NbrOfConversion = 1;  // only one conversion needed
  ADC_Init(ADC1, &ADCInitStruct);
  ADC_Init(ADC2, &ADCInitStruct);
  ADC_Init(ADC3, &ADCInitStruct);

  // turn on ADC
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
}

uint16_t REC_readValue(ADC_TypeDef* ADCx, uint8_t ADC_channel) {
  // set pin to read
  ADC_RegularChannelConfig(ADCx, ADC_channel, 1, ADC_SampleTime_3Cycles);
  // start reading and wait until finished
  ADC_SoftwareStartConv(ADCx);
  while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
  // return converted value
  return ADC_GetConversionValue(ADCx);
}

/******************** RECEIVER MACROS *************************/
// NOTE THAT THESE ARE NOT CALIBRATED. THEY CAN BE NOISY!
#define READ_LEFT_REC      REC_readValue(ADC2, ADC_Channel_9)
#define READ_BLEFT_REC     REC_readValue(ADC3, ADC_Channel_12)
#define READ_FLEFT_REC     REC_readValue(ADC3, ADC_Channel_11)
#define READ_FRONT_REC     REC_readValue(ADC3, ADC_Channel_10)
#define READ_FRIGHT_REC    REC_readValue(ADC1, ADC_Channel_14)
#define READ_BRIGHT_REC    REC_readValue(ADC1, ADC_Channel_8)
#define READ_RIGHT_REC     REC_readValue(ADC2, ADC_Channel_13)
/**************************************************************/
#ifdef __cplusplus
  }
#endif

#endif /* RECEIVERMACROS_H_ */
