/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @brief   Example usage of the ADC. It's quite complex to use (and very inconvenient
  *          to wrap around. However, once set up, all reading is quite simple to
  *          do (whether using direct ADCx->DR access or the CMSIS functions)
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "Timer.h"
#include "HardwareSerial.h"

void setupADC(void);

int main(void)
{
  setupADC();
  TIM_Delay_Init();
  HUSART_begin(USART2, 9600);
  double recordedVal;

  while(1) {
    ADC_SoftwareStartConv(ADC1);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
    recordedVal = ((int)ADC_GetConversionValue(ADC1))*3.3/4095;  // note 3.3V is the reference voltage! 4096 is conversion level
    HUSART_writeString(USART2, "ADC Value: ");
    HUSART_writeFloat(USART2, recordedVal);
    HUSART_writeChar(USART2, '\n');
    TIM_Delay_Micro(100);
  }
}

void setupADC(void) {
  // enable clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // set up pin for ADC use
  GPIO_InitTypeDef GPIOInitStruct;
  GPIOInitStruct.GPIO_Pin = GPIO_Pin_4;   // init PA4
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIOInitStruct);

  // set up ADC
  ADC_DeInit();   // makes sure ADC is off beforehand

  ADC_CommonInitTypeDef ADCComStruct;
  ADCComStruct.ADC_Mode = ADC_Mode_Independent;
  ADCComStruct.ADC_Prescaler = ADC_Prescaler_Div2;
  ADCComStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
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
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);   // sample channel 4 using ADC1 first for 3 cycles
  ADC_Cmd(ADC1, ENABLE);
}
