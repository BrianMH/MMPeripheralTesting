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
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "HardwareSerial.h"

int main(void)
{
	// Set up necessary clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Set up GPIO ports
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);	// inits to ALLPIN/Mode_In/2MHz/Push-Pull/NoPuPd
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Set up DMA for ADC
    DMA_DeInit(DMA1_Stream0); // Reset DMA configuration
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);

	// Set up ADC
    ADC_DeInit();	// Reset current adc setup
    ADC_InitTypeDef ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);
    // First time setting up ADC, so write out all values
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	// required for DMA use
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	// data is least sig bit -> most sig
//    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; // unused?
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // software started conversion
    ADC_InitStruct.ADC_NbrOfConversion = 2;	// Place how many channels will be read by the adc
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE; // more than one channel scanned
    ADC_Init(ADC1, &ADC_InitStruct);


    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_CommonStructInit(&ADC_CommonInitStruct);
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

	while(1) {
		// Code that repeats
	}
}
