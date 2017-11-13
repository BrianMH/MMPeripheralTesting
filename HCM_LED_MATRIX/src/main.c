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
#include "hcm_display.h"

int main(void)
{
	// initialize display
	HCM_init();
	HCM_setBrightness(1);

	for(;;) {
		HCM_print(" tst");
		HCM_print("tst");
	}
}
