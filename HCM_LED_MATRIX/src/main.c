/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @brief   Implements basic usage of the HCM display using the created libraries
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
