/**
  ******************************************************************************
  * @file    TimerMS.h
  * @author  Kenmei
  * @brief   Hardware based timer that allows timer usage! Note that interrupts
  * 		 may actually cause this timer to slightly deviate from its intended
  * 		 delay
  ******************************************************************************
*/

#ifndef __TIMER
#define __TIMER

#ifdef __cplusplus
  extern "C" {
#endif

volatile uint32_t __multiplier;	// #instr/time

/**
 * Initializes timer for usage
 */
void TIM_Delay_Init();

/**
 * Delays the program by a specified amount of microseconds
 * @param msecs - the duration to stall execution for (in us)
 */
void TIM_Delay_Micro(uint32_t msecs);

/**
 * Delays the program by a specified amount of milliseconds
 * @param msecs - the duration to stall execution for (in ms)
 */
void TIM_Delay_Milli(uint32_t msecs);

#ifdef __cplusplus
  }
#endif

#endif
