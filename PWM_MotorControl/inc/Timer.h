/**
  ******************************************************************************
  * @file    Timer.h
  * @author  Kenmei
  * @brief   Really simple timer module. Seriously, just read it...
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
 * Delays the program by a specified amount of milliseconds
 * @param msecs - the duration to stall execution for (in ms)
 */
void TIM_Delay_Micro(uint32_t msecs);

#ifdef __cplusplus
  }
#endif

#endif
