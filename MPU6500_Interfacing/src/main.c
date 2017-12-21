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

// Defines
#define _CLOCK_PIN GPIO_Pin_13    // PB13
#define _DATAIN_PIN GPIO_Pin_14   // PB14
#define _DATAOUT_PIN GPIO_Pin_15  // PB15
#define _CS_PIN GPIO_Pin_1   	  // PB1
#define BUF_SIZE 12

// Registers
#define USER_CTRL_REG 0x6A

// Includes
#include "stm32f4xx.h"
#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>

// Structures
typedef struct IMU_vals{
	int acc_x;
	int acc_y;
	int acc_z;
	int gyro_x;
	int gyro_y;
	int gyro_z;
} IMU_vals;

// Some helper methods to use
void MPU6500_Init() {
	// Init clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;
	// establish tx/rx/clk pins as SPI pins
	GPIO_InitTypeDefStruct.GPIO_Pin = _CLOCK_PIN | _DATAIN_PIN | _DATAOUT_PIN;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	// CS pin (enable/disable) pin is a regular pin
	GPIO_InitTypeDefStruct.GPIO_Pin = _CS_PIN;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;

	// Initialize SPI capabilities (See MPU6500 data sheet, p.34, for more information)
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// tx/rx duplex
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;	// 1 byte per tx
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;   // data latched on rising edge
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;   // data transitioned on falling edge
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;	// unused so doesn't matter
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;   // maximum SCLK of 1MHz
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;  // Data is delivered on MSB first
	SPI_InitTypeDefStruct.SPI_CRCPolynomial = 7;   // unused since no CRC available
	SPI_Init(SPI2, &SPI_InitTypeDefStruct);
	SPI_Cmd(SPI2, ENABLE);

	// Ensure module does not enter I2C mode
	MPU6500_WriteToReg(USER_CTRL_REG, 0b00100000);
}

void MPU6500_WriteToReg(uint8_t address, uint8_t data) {
	SPI_I2S_SendData(SPI2, 0x00|address);   // sets the mode to denote write to add
	SPI_I2S_SendData(SPI2, data);  			// one byte of data is sent
}

/**
 * This method can possibly generate memory leaks!!!!! Use it wisely...
 */
uint8_t* MPU6500_ReadFromReg(uint8_t address, uint8_t num_bytes) {
	if(num_bytes<1)
		return (void*)0;

	uint8_t buffer[BUF_SIZE] = malloc(sizeof(uint8_t)*BUF_SIZE);	// create buffer
	SPI_I2S_SendData(SPI2, 0x80|address);	// sets the mode to denote read from add

	for(uint8_t count=0; count<num_bytes; count++) {
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)) {}	// wait for possible tx/rx to end
		buffer[count] = SPI_I2S_ReceiveData(SPI2);    // burst read for any given bytes
	}

	return &buffer;   // make sure to free this!
}

int main(void)
{
	MPU6500_Init();

	// While loop
	for(;;) {
		;
	}
}
