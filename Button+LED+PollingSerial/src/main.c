/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @date    17-08-15
  * @brief   A generic example of using an LED (GPIO output), a button (GPIO Input),
  *          and the USART peripherals of the STM32 board.
  ******************************************************************************
*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "Timer.h"

typedef enum { false, true } bool;  // Set up boolean values

// Initializes all the peripherals used in this program (because who wants all this in main...)
void initPeripherals(void) {
	// Enable Peripheral Clocks (Multi Init)
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

   // Configure Pins
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_StructInit(&GPIO_InitStructure);  // Generate generic init structure
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // PA5 is LED
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   // Pin is an output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   // Speed not important
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Push-pull operation (default)
   GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize PA5 according to above

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   // Like above, but now for button
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   // Init and Set up serial interface USART2
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOA, &GPIO_InitStructure); // Sets up Tx
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_Init(GPIOA, &GPIO_InitStructure); // Sets up Rx
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

   USART_InitTypeDef USART_InitStructure;
   USART_StructInit(&USART_InitStructure);   // Generate generic init structure
   USART_InitStructure.USART_BaudRate = 9600;   // 9600 Baud
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8bit word lengths
   USART_InitStructure.USART_StopBits = USART_StopBits_1;   // 1 stop bit
   USART_InitStructure.USART_Parity = USART_Parity_No;   // No parity bits
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // No Flow Cntrol
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // Rx/Tx Mode
   USART_Init(USART2, &USART_InitStructure);
   USART_Cmd(USART2, ENABLE); // Start up USART2
}

/**
 * The main function...
 */
int main(void) {
	// Initialize peripherals
	initPeripherals();

   // Init simple timer
   TIM_Delay_Init();

   // Effectively the "loop" part of main
   USART_SendData(USART2, '5');
   bool button_status = false;
   while(1) {
      button_status = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

      if(button_status) {  // If button has been pressed
         volatile static char c;
         if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {   // Check if something has been received
            c = (char)USART_ReceiveData(USART2);
            USART_SendData(USART2, c); // echo what was received
            USART_ClearFlag(USART2, USART_FLAG_RXNE); // Wait for something else
         }

         if(c == 'O') { // Turn LED on for 2 seconds if you receive an 'O'
            GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
            TIM_Delay_Micro(2000);
            GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
         }
      } else { // If button not pressed send a character
         USART_SendData(USART2, '.');
         TIM_Delay_Micro(2000);
      }
   }
}
