/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @date    17-08-15
  * @brief   This program serves to show the most basic elements of a program
  *          written using the STM32F4 Standard Peripheral Libraries. It blinks
  *          an LED using a 1sec hold and interrupts.
  *          NOTE: In general, it is undesired to use software delays as it can
  *                stall the program. If the SysTick interrupt is not prioritized,
  *                the program may stall.
  ******************************************************************************
*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

/**
 * @brief   Delays the program by a specified amount of time.
 * @param   Time to stall program for in milliseconds
 * @retval  None
 */
void soft_delay(uint32_t nTime);

/**
 * The main function...
 */
int main(void)
{
   // Enable Peripheral Clocks
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // PA5 is LED. Turn on PA ports

   // Configure Pins
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_StructInit(&GPIO_InitStructure);  // Generate init structure
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // PA5 is LED
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   // Pin is an output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   // Speed not important
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Push-pull operation (default)
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // Pull signal down on float
   GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize PA5 according to above

   // Configure SysTick Timer
   if(SysTick_Config(SystemCoreClock/1000))  // Every 1msec, SysTick interrupt occurs
      while(1){
         ;  // Stall if configuration fails
      }

   // Effectively the "loop" part of main
   while(1) {
      static int ledval = 0;

      // toggle led
      GPIO_WriteBit(GPIOA, GPIO_Pin_5, (ledval)?Bit_SET:Bit_RESET);
      ledval = 1-ledval;

      soft_delay(1000); // wait specified amount of msec (1sec)
   }
}

/***************** PRIVATE FUNCTIONS AND VARIABLES **********************/
static __IO uint32_t TimingDelay;   // Prevent optimization of counter

void soft_delay(uint32_t nTime) {
   TimingDelay = nTime;
   while(TimingDelay!=0){
      ;  // Wait for interrupts to reduce counter
   }
}

// ISR for SysTick
void SysTick_Handler(void) {
   if(TimingDelay!=0x00)
      TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
   /* Infinite Loop */
   /* Use GDB to find out why we're here */
   while(1);
}
#endif
