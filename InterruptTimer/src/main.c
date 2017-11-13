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
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "HardwareSerial.h"

int main(void)
{
	// set up serial interface
	HUSART_begin(USART2, 9600);	// usart2 is wired to usb

	// init structs
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable rcc clocks for peripherals
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Configure pin inputs
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	// Timer overflows back to 0 when at 0xff...
	TIM_SetAutoreload (TIM2, 0xffffffff);

	/* Configure the timer */
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	/* TIM2 counter enable */
	TIM_Cmd(TIM2, ENABLE);

	// main code
	uint32_t counter = TIM2->CNT;

	while(1) {
		if(TIM2->CNT - counter > 100) { // message every five ticks
			HUSART_writeUInt(TIM2->CNT);
			counter = TIM2->CNT;
		}
	}
}
