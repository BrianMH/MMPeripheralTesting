/*
 * delay.h
 *
 *  Created on: Sep 22, 2017
 *      Author: Brian
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

static __IO uint32_t TimingDelay;

void delay_init();

void delay(uint32_t nTime);

void delayNano(uint32_t nTime);

void SysTick_Handler();

#endif /* DELAY_H_ */
