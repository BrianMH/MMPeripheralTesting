/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @version V1.0
  * @date    01-December-2013
  * @brief   Testing Attempts for the MPU6500 Module. This will just be testing
  * 		 connectivity and eventually become a library. [Note that this will
  * 		 be coded with the module being connected to the SPI2 pins]
  * 		 NOTE: THIS LIBRARY REQUIRES THE TIMER LIBRARY TO BE INCLUDED!!!
  ******************************************************************************
*/

// Includes
#include "stm32f4xx.h"
#include <HardwareSerial.h>
#include "MPU6500.h"
#include "Timer.h"

int main(void)
{
   TIM_Delay_Init();
	HUSART_begin(USART3, 9600);
	HUSART_writeString(USART3, "Starting...\n");
	MPU6500_Init();

	// perma loop
	for(;;) {
		HUSART_writeString(USART3, "acc_x :");
		HUSART_writeInt(USART3, MPU6500.r_acc_x);
		HUSART_writeString(USART3, "\t\tacc_y :");
		HUSART_writeInt(USART3, MPU6500.r_acc_y);
		HUSART_writeString(USART3, "\t\tacc_z :");
		HUSART_writeInt(USART3, MPU6500.r_acc_z);
		HUSART_writeString(USART3, "\t\tgyro_x:");
		HUSART_writeInt(USART3, MPU6500.r_gyro_x);
		HUSART_writeString(USART3, "\t\tgyro_y:");
		HUSART_writeInt(USART3, MPU6500.r_gyro_y);
		HUSART_writeString(USART3, "\t\tgyro_z:");
		HUSART_writeInt(USART3, MPU6500.r_gyro_z);
		HUSART_writeString(USART3, "\n");

		// get new values
//		MPU6500_UpdateValues();
		MPU6500_GetAccGyroValues();
		TIM_Delay_Micro(100);   // timer was already initialized
		                        // but it's find if it is reinitialized
		                        // since the user won't know...
	}
}
