#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>
#include "MPU6500.h"
#ifdef MPU_DEBUG_ON
#include <HardwareSerial.h>
#endif

/*************** TIMER METHODS ***********************/
void TIM_Delay_Init(){
	RCC_ClocksTypeDef RCC_Clocks;

	// Use internal clock to scale delays
	RCC_GetClocksFreq(&RCC_Clocks);
	__multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

void TIM_Delay_Micro(uint32_t msecs) {
	uint32_t millis = 1000*msecs*__multiplier - 10;
	while(millis--) {}; // delay happens here
}

/************** MPU6500 METHODS ***********************/
void MPU6500_WriteToReg(uint8_t address, uint8_t data) {
	 GPIO_ResetBits(GPIOB, _CS_PIN);

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));	// wait until tx buffer empty
	SPI_I2S_SendData(SPI2, address);	// send a packet
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)); // wait until rx buffer not empty
	SPI_I2S_ReceiveData(SPI2);

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, data);
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI2);

	GPIO_SetBits(GPIOB, _CS_PIN);
}

uint8_t MPU6500_ReadFromReg(uint8_t address) {
	GPIO_ResetBits(GPIOB, _CS_PIN);

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, 0x80|address);
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI2); //Clear RXNE bit

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, 0x00); //Dummy byte to generate clock
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));

	GPIO_SetBits(GPIOB, _CS_PIN);
	return SPI_I2S_ReceiveData(SPI2);
}

// So called "half operation" for usage in burst reading
uint8_t MPU6500_Transfer(uint8_t data, SPITransferMode t_mode) {
	GPIO_ResetBits(GPIOB, _CS_PIN);

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, data);
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));

	if(t_mode == SPI_LAST)
		GPIO_SetBits(GPIOB, _CS_PIN);

	return SPI_I2S_ReceiveData(SPI2);
}

// TODO: Burst read is EXTREMELY prone to read mistakes. It needs a lot more investigation before using.
void MPU6500_GetAccGyroValues() {
//	uint8_t bytes[12] = {0};
//
//	//read in bytes
//	MPU6500_Transfer(0x80|MPU6500_RA_FIFO_R_W, SPI_CONTINUE);
//	for(uint8_t i=0; i<12-1; i++) {
//		bytes[i] = MPU6500_Transfer(0x00, SPI_CONTINUE); // send dummy bytes for reading
//	}
//	bytes[12-1] = MPU6500_Transfer(0x00, SPI_LAST);
//
//	// now assign them to the value struct
//	MPU6500.acc_x = (int16_t)((bytes[0]<<8)+bytes[1]);
//	MPU6500.acc_y = (int16_t)((bytes[2]<<8)+bytes[3]);
//	MPU6500.acc_z = (int16_t)((bytes[4]<<8)+bytes[5]);
//	MPU6500.gyro_x = (int16_t)((bytes[6]<<8)+bytes[7]);
//	MPU6500.gyro_y = (int16_t)((bytes[8]<<8)+bytes[9]);
//	MPU6500.gyro_z = (int16_t)((bytes[10]<<8)+bytes[11]);

	// read in bytes manually through registers
	MPU6500.r_acc_x = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_H)<<8)+MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L));
	MPU6500.r_acc_y = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_YOUT_H)<<8)+MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L));
	MPU6500.r_acc_z = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_ZOUT_H)<<8)+MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L));
	MPU6500.r_gyro_x = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_H)<<8)+MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L));
	MPU6500.r_gyro_y = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_YOUT_H)<<8)+MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L));
	MPU6500.r_gyro_z = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_ZOUT_H))+MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L));
}

void MPU6500_UpdateValues(){
	// read in bytes manually through registers (see above about burst reading)
	MPU6500.acc_x = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_H)<<8)+
			MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L))/__ACC_CONV;
	MPU6500.acc_y = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_YOUT_H)<<8)+
			MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L))/__ACC_CONV;
	MPU6500.acc_z = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_ACCEL_ZOUT_H)<<8)+
			MPU6500_ReadFromReg(MPU6500_RA_ACCEL_XOUT_L))/__ACC_CONV;
	MPU6500.gyro_x = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_H)<<8)+
			MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L))/__GYRO_CONV;
	MPU6500.gyro_y = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_YOUT_H)<<8)+
			MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L))/__GYRO_CONV;
	MPU6500.gyro_z = (int16_t)((MPU6500_ReadFromReg(MPU6500_RA_GYRO_ZOUT_H))+
			MPU6500_ReadFromReg(MPU6500_RA_GYRO_XOUT_L))/__GYRO_CONV;
}

void MPU6500_Init() {
	// Init timer
	TIM_Delay_Init();
#ifdef MPU_DEBUG_ON
	// check if USART is already on
	if((USART2->CR1 & ((uint16_t)USART_CR1_UE)) != ((uint16_t)USART_CR1_UE))
		HUSART_begin(USART2, _DEFAULT_BAUD);
#endif

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
	GPIO_PinAFConfig(GPIOB, _CLOCK_SOURCE, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, _DATAIN_SOURCE, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, _DATAOUT_SOURCE, GPIO_AF_SPI2);

	// and set up SS pin
	GPIO_InitTypeDefStruct.GPIO_Pin = _CS_PIN;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
	GPIO_SetBits(GPIOB, _CS_PIN);

	// Initialize SPI capabilities (See MPU6500 data sheet, p.34, for more information)
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// tx/rx
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;	// 1 byte per tx
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;   // data latched on rising edge
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;   // data transitioned on falling edge
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;   // maximum SCLK of 1MHz
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;  // Data is delivered on MSB first
	SPI_InitTypeDefStruct.SPI_CRCPolynomial = 0;   // unused since no CRC available
	SPI_Init(SPI2, &SPI_InitTypeDefStruct);
	SPI_Cmd(SPI2, ENABLE);

	// Places a delay for the loop to set itself up and then allow reading from device
	TIM_Delay_Micro(1000);

	/** The following reset procedure is defined on p. 42 of the MPU6500 register map**/
	// Reset device
	MPU6500_WriteToReg(MPU6500_RA_PWR_MGMT_1, 0x80);
	TIM_Delay_Micro(100);
	// Reset sensors
	MPU6500_WriteToReg(MPU6500_RA_SIGNAL_PATH_RESET, 0x05);
	TIM_Delay_Micro(100);
	// Finally set SPI mode permanently and reset other components
	MPU6500_WriteToReg(MPU6500_RA_USER_CTRL, 0x10|0x08|0x04|0x01);
	TIM_Delay_Micro(1000);

#ifdef MPU_DEBUG_ON
	// Tiny self-reassurance using WHO_AM_I register
	uint8_t id = MPU6500_ReadFromReg(MPU6500_RA_WHO_AM_I);
	HUSART_writeInt(USART2, id);	// should be 112
	HUSART_writeString(USART2, "\tend\n");
#endif

	// Now configure various settings for sensors (see register map)
	// By default, accelerometer has 1kHz sampling rate w/ 460 Hz bandwidth
	// By default, gyroscope has 250 Hz bandwidth w/ 4kHz sampling rate (+250 dps)
	MPU6500_WriteToReg(MPU6500_RA_CONFIG, 0x01); // set gyro to 184hz bandwidth
	MPU6500_WriteToReg(MPU6500_RA_FIFO_EN, MPU6500_FIFO_EN_ACC | MPU6500_FIFO_EN_GYRO); // enable accelerometer and gyro FIFO

	// Finally, enable FIFO buffer and disable I2C switching
	MPU6500_WriteToReg(MPU6500_RA_USER_CTRL, 0x40|0x10);
}
