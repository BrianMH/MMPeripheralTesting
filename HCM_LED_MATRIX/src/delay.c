/*
 * delay.c
 *
 *  Created on: Sep 22, 2017
 *      Author: Brian
 */
#include "delay.h"
#include "stm32f4xx.h"

void delay_init() {
	SysTick_Config(SystemCoreClock/1000); // sets up timer
}

void delay(uint32_t nTime) {
	TimingDelay = nTime;	// turns time in ms
	while(TimingDelay != 0) {}
}

void SysTick_Handler() {
	if(TimingDelay != 0) {
		TimingDelay--;
	}
}


