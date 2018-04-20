/*
 * EmitterMacros.h
 *
 *  Created on: Apr 20, 2018
 *      Author: brian
 */

#ifndef EMITTERMACROS_H_
#define EMITTERMACROS_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#ifdef __cplusplus
  extern "C" {
#endif

// Configures all of the emitters
// Locations are noted below for reference (CW):
//    IR-L  -----> PD2
//    IR-FL -----> PC11
//    IR-F  -----> PC10
//    IR-FR -----> PA8
//    IR-R  -----> PC7

void EMIT_Init() {
  // initialize clocks for all emitters
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

  // init with gpio struct
  GPIO_InitTypeDef GPIOInitStruct;
  GPIOInitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOD, &GPIOInitStruct);      // INIT PD2

  GPIOInitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIOInitStruct);      // INIT PC7/10/11

  GPIOInitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIOInitStruct);      // INIT PA8
}

void EMIT_SideEmittersOn() {
  GPIO_SetBits(GPIOD, GPIO_Pin_2);  // left
  GPIO_SetBits(GPIOC, GPIO_Pin_7);  // right
}

void EMIT_SideEmittersOff() {
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);   // left
  GPIO_ResetBits(GPIOC, GPIO_Pin_7);   // right
}

/******************************** MACRO DEFINITIONS ********************************/
/*
 * In general, the process for reading from these should be as follows:
 *    1) Turn on emitter
 *    2) Wait 60us
 *    3) Query ADC for measurement and read it
 *    4) Turn off emitter
 * This ensures that the emitter life is prolonged since it is being run at a higher
 * current than its typical one. [Side emitter reading should be done in unison]
 */

// ON OPERATIONS
#define IR_FRONT_ON     GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define IR_FLEFT_ON     GPIO_SetBits(GPIOC, GPIO_Pin_11)
#define IR_FRIGHT_ON    GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define IR_SIDES_ON      EMIT_SideEmittersOn()

// OFF OPERATIONS
#define IR_FRONT_OFF    GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define IR_FLEFT_OFF    GPIO_ResetBits(GPIOC, GPIO_Pin_11)
#define IR_FRIGHT_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define IR_SIDES_OFF     EMIT_SideEmittersOff()
/***********************************************************************************/

#ifdef __cplusplus
  }
#endif

#endif /* EMITTERMACROS_H_ */
