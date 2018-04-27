/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @brief   Implements basic usage of the HCM display using the created libraries
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "hcm_display.h"
#include "Timer.h"

int main(void)
{
	// initialize display
	HCM_init();
	HCM_setBrightness(10);

	// initialize timer
	TIM_Delay_Init();

	for(;;) {
		HCM_print("test");
		TIM_Delay_Micro(100);
		HCM_print("test");
		TIM_Delay_Micro(100);
	}
}
