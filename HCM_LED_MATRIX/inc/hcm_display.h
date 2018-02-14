/*
 * hcm_display.h
 *
 * NOTE: THE PINOUT FOR THIS CODE IS AS FOLLOWS:
 * 			PB15 --> DATAIN
 * 			PB13 --> CLOCK
 * 			PC7  --> REGISTER SELECT
 * 			PC14 --> CHIP ENABLE
 *
 */

#ifndef HCM_DISPLAY_H_
#define HCM_DISPLAY_H_

#define _HCM_USE_SPI	// USED AS REFERENCE DEFINE. COMMENT UNDEFINE TO USE SPI
//#undef _HCM_USE_SPI

#define _HCM_USE_LEGACY
#undef  _HCM_USE_LEGACY	// removes legacy options (AKA those from originally ported ver)

/****************************** INCLUDES ********************************/
#include <stm32f4xx_usart.h>

/****************************** DEFINES ********************************/
#define _MAXSTRINGLENGTH 4	// 4 digit display
#define _LEDDISPLAY_MAXCHARS  32	// can be extended to 32
#define _DATA_PIN GPIO_Pin_15		// NOTE: DATA/CLK SHOULD BE ON B BUS PINS!!!
#define _CLOCK_PIN GPIO_Pin_13
#define _RS_PIN GPIO_Pin_7			// NOTE: RS/CE SHOULD BE ON C BUS PINS!!!
#define _CE_PIN GPIO_Pin_8

/****************************** VARIABLES ********************************/
static __IO int8_t _cursorPos;
static __IO uint8_t _dotRegister[_MAXSTRINGLENGTH*5+1] = {0};
//static uint8_t _prevBrightness = 15;

/****************************** FUNCTIONS ********************************/
/**
 * Initializes the pins for the display as indicated at the top of this header file.
 */
void HCM_init();

/**
 * 	Refreshes the display to display whatever is in the dotRegister array
 */
void HCM_loadDotRegister();

/**
 * Sends 8bits to one of the control registers
 * @param dataByte - the data to send to the control register
 */
void HCM_loadControlRegister(uint8_t dataByte);

/**
 * Sets the brightness (Only use values in between 0 - 15 !!!!!)
 * @param bright - the value desired for display brightness
 */
void HCM_setBrightness(uint8_t bright);

/**
 * Loads bits into the dot register array. THIS FUNCTION DOES NOT WRITE TO THE DISPLAY!!!!
 * @param whatCharacter - the character to load to the array
 * @param whatPosition - the position to place the character at
 */
void HCM_writeCharacter(char whatCharacter, uint8_t whatPosition);

/**
 * Sets the display cursor back to the home position (0).
 */
void HCM_setCursorHome();

/**
 * Sets the display cursor to a certain position (ideally within the amount of digits you have)
 * @param whichPos - determines the cursor position
 */
void HCM_setCursorPos(uint8_t whichPos);

/**
 * Returns the current position of the display cursor
 * @return the position of the display cursor
 */
int HCM_getCursorPos();

/**
 * Writes a byte out to the display and advances the cursor position
 * @param b - the character to write to the screen
 */
void HCM_write(uint8_t b);

/**
 * Clears the screen of all characters
 */
void HCM_clear();

/**
 * An improved version of the write function above that takes strings and prints them to the screen.
 * Strings longer than four characters are clipped to four chars, and strings smaller than four
 * characters are simply padded with spaces after the string.
 * @param string - the string to be printed
 */
void HCM_print(char* string);

/**
 * An improved version of the write function that takes integer values and converts it to the closest
 * four digit approximation. Float values are not supported, so take care to convert it into an integer
 * before printing it.
 * @param val - the value to be printed
 */
void HCM_printInt(int val);

void HCM_printFloat(float val, uint8_t mantissa);

#ifndef _HCM_USE_SPI
void HCM_shiftOut(uint8_t byte);
#endif

#endif /* HCM_DISPLAY_H_ */
