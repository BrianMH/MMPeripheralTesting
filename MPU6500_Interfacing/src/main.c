/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @version V1.0
  * @date    01-December-2013
  * @brief   Testing Attempts for the MPU6500 Module. This will just be testing
  * 		 connectivity and eventually become a library. [Note that this will
  * 		 be coded with the module being connected to the SPI2 pins]
  ******************************************************************************
*/

// Includes
#include "stm32f4xx.h"
#include <HardwareSerial.h>
#include "MPU6500.h"

int main(void)
{
	HUSART_begin(USART2, 115200);
	MPU6500_Init();

	// perma loop
	for(;;) {
		HUSART_writeString(USART2, "acc_x :");
		HUSART_writeFloat(USART2, MPU6500.acc_x);
		HUSART_writeString(USART2, "\t\tacc_y :");
		HUSART_writeFloat(USART2, MPU6500.acc_y);
		HUSART_writeString(USART2, "\t\tacc_z :");
		HUSART_writeFloat(USART2, MPU6500.acc_z);
		HUSART_writeString(USART2, "\t\tgyro_x:");
		HUSART_writeFloat(USART2, MPU6500.gyro_x);
		HUSART_writeString(USART2, "\t\tgyro_y:");
		HUSART_writeFloat(USART2, MPU6500.gyro_y);
		HUSART_writeString(USART2, "\t\tgyro_z:");
		HUSART_writeFloat(USART2, MPU6500.gyro_z);
		HUSART_writeString(USART2, "\n");

		// get new values
		MPU6500_UpdateValues();
		TIM_Delay_Micro(100);
	}
}
