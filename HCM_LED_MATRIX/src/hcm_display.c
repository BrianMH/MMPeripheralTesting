// includes
#include "hcm_display.h"
#include "font5x7.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#ifdef _HCM_USE_SPI
#include <stm32f4xx_spi.h>
#endif

void HCM_init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	#ifdef _HCM_USE_SPI
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	#endif

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;
	#ifdef _HCM_USE_SPI
		// establish tx/clk pins as SPI pins
		GPIO_InitTypeDefStruct.GPIO_Pin = _CLOCK_PIN | _DATA_PIN;
		GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	#else
		// establish tx/clk pins as regular output pins
		GPIO_InitTypeDefStruct.GPIO_Pin = _CLOCK_PIN | _DATA_PIN;
		GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
	#endif

	// establish RS/CE pins as regular output pins
	GPIO_InitTypeDefStruct.GPIO_Pin = _RS_PIN | _CE_PIN;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitTypeDefStruct);

	#ifdef _HCM_USE_SPI
		// Initialize spi struct
		SPI_InitTypeDef SPI_InitTypeDefStruct;
		SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_1Line_Tx;	// master tx only
		SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
		SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;	// 1 byte per tx
		SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;	// unused so doesn't matter
		SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitTypeDefStruct.SPI_CRCPolynomial = 7;
		SPI_Init(SPI2, &SPI_InitTypeDefStruct);
		SPI_Cmd(SPI2, ENABLE);
	#endif

	// set dot register to all lows
	HCM_loadDotRegister();

	HCM_loadControlRegister(0b10000001); // set serial mode. see table 1, footnote 1
	// set control register 0 for max brightness, and no sleep:
	HCM_loadControlRegister(0b01111111);
}

#ifndef _HCM_USE_SPI
void HCM_shiftOut(uint8_t byte) {
	// shifts out the byte starting from the most significant bit
	for(uint8_t i=0; i<8; i++) {
		uint8_t toWrite = !!(byte & (1 << (7-i)));
		GPIO_WriteBit(GPIOB, _DATA_PIN, toWrite);
		GPIO_SetBits(GPIOB, _CLOCK_PIN);		// clock tick
		GPIO_ResetBits(GPIOB, _CLOCK_PIN);
	}
}
#endif

void HCM_loadDotRegister() {
	// define max data to send
	int maxData = _MAXSTRINGLENGTH * 5;

	// select dot register
	GPIO_ResetBits(GPIOC, _RS_PIN);
	// enable writing to the display
	GPIO_ResetBits(GPIOC, _CE_PIN);
	//shift data out
	for(int i=0; i<maxData; i++) {
		#ifdef _HCM_USE_SPI
			SPI_I2S_SendData(SPI2, _dotRegister[i]);
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)) {}
		#else
			HCM_shiftOut(_dotRegister[i]);
		#endif
	}
	//disable writing
	GPIO_SetBits(GPIOC, _CE_PIN);
}

void HCM_loadControlRegister(uint8_t dataByte) {
	// select control register
	GPIO_SetBits(GPIOC, _RS_PIN);
	// enable writing to the display
	GPIO_ResetBits(GPIOC, _CE_PIN);
	// shift data out
	#ifdef _HCM_USE_SPI
		SPI_I2S_SendData(SPI2, dataByte);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)) {}
	#else
		HCM_shiftOut(dataByte);
	#endif
	// disable writing
	GPIO_SetBits(GPIOC, _CE_PIN);
}

void HCM_setBrightness(uint8_t bright) {
	// keeps previous brightness
	_prevBrightness = 0b01110000 + bright;

	// sets brightness
	HCM_loadControlRegister(_prevBrightness);
}

void HCM_writeCharacter(char whatCharacter, uint8_t whatPosition) {
	// calculate the starting position in the array.
	// every character has 5 columns made of 8 bits:
	uint8_t thisPos = whatPosition*5;

	// copy appropriate bits into dot register array
	for(int i=0; i<5; i++) {
		_dotRegister[thisPos+i] = Font5x7[((whatCharacter - 0x20) * 5) + i];
	}
}

void HCM_print(char* string) {
	// LED refresh occasionally stalls. This helps prevent that on most occasions
	HCM_loadControlRegister(_prevBrightness);

	char curr;

	// position no longer important, simply prints out any four digit string
	for(int i=0; i<_MAXSTRINGLENGTH; i++) {
		curr = string[i];
		if(curr == '\0') {	// ends at null char
			for(int k=i*5; k<_MAXSTRINGLENGTH*5+1; k++)
				_dotRegister[k] = 0;
			break;
		}

		for(int j=0; j<5; j++) {
			_dotRegister[(i * 5) + j] = Font5x7[((curr - 0x20) * 5) + j];
		}
	}

	HCM_loadDotRegister();
}

void HCM_printInt(int val) {
	// temp string holder
	char temp[4] = {0};
	int rtmath = val;

	// convert value
	for(int i=0; i<_MAXSTRINGLENGTH; i++) {
		temp[i] = rtmath%10;
		rtmath/=10;
	}

	// now print it
	HCM_print(temp);
}

void HCM_setCursorHome() {
	_cursorPos = 0;
}

void HCM_setCursorPos(uint8_t whichPos) {
	if(whichPos >= 0 && whichPos <= _MAXSTRINGLENGTH)
		_cursorPos = whichPos;
}

int HCM_getCursorPos() {
	return _cursorPos;
}

void HCM_clear() {
	for(int displayPos = 0; displayPos < _MAXSTRINGLENGTH; displayPos++)
		HCM_writeCharacter(' ', displayPos);

	// send the dot register array out to the display
	HCM_loadDotRegister();
}

void HCM_write(uint8_t b) {
	// make sure cursorpos is on the display
	if(_cursorPos >=0 && _cursorPos < _MAXSTRINGLENGTH) {
		// put character into the dot register
		HCM_writeCharacter(b, _cursorPos);
		_cursorPos++;

		// send the dot register out to the display
		HCM_loadDotRegister();
	}
}
