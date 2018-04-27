/**
  ******************************************************************************
  * @file    DualEncoder.h
  * @author  Kenmei
  * @brief   Implementation of the encoders. It's largely controlled through the
  *          wall of defines in the top. Currently counts all four edges of the
  *          quadrature encoder, but it can be set to only count half as many
  *          through the initialization portion.
  *
  *          TODO: Improve documentation
  ******************************************************************************
*/

#ifndef DUALENCODER_H_
#define DUALENCODER_H_

#include "stm32f4xx.h"

#ifdef __cplusplus
  extern "C" {
#endif

/****************************** ENCODER PINOUT ***************************************
 * Encoders are currently wired as follows (and these functions assume so through defines):
 *
 * RIGHT MOTOR - ENCODER CHA/CHB (TIM2 CH1,2) [PA15/PB3]
 * LEFT MOTOR - ENCODER CHA/CHB (TIM5 CH1,2) [PA0/PA1]
 *
 * An ISR service routine is also defined and may also be called by the main loop.
 *************************************************************************************/
// Left Motor
#define _ENCLA_PIN         GPIO_Pin_0
#define _ENCLA_GPIO_PORT   GPIOA
#define _ENCLA_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define _ENCLA_SOURCE      GPIO_PinSource0
#define _ENCLA_AF          GPIO_AF_TIM5

#define _ENCLB_PIN         GPIO_Pin_1
#define _ENCLB_GPIO_PORT   GPIOA
#define _ENCLB_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define _ENCLB_SOURCE      GPIO_PinSource1
#define _ENCLB_AF          GPIO_AF_TIM5

// Right Motor
#define _ENCRA_PIN         GPIO_Pin_15
#define _ENCRA_GPIO_PORT   GPIOA
#define _ENCRA_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define _ENCRA_SOURCE      GPIO_PinSource15
#define _ENCRA_AF          GPIO_AF_TIM2

#define _ENCRB_PIN         GPIO_Pin_3
#define _ENCRB_GPIO_PORT   GPIOB
#define _ENCRB_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define _ENCRB_SOURCE      GPIO_PinSource3
#define _ENCRB_AF          GPIO_AF_TIM2

// Timer Definitions
#define _ENCL_TIMER         TIM5
#define _ENCL_TIMER_CLK_EN  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE)
#define _ENCR_TIMER         TIM2
#define _ENCR_TIMER_CLK_EN  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE)

/***************************** GLOBAL VARIABLES **********************************/
struct DistStruct {
  int32_t leftTotal;
  int32_t rightTotal;
  int32_t fwdTotal;
  int32_t rotTotal;
};

struct SpeedStruct {
  int32_t leftCount;
  int32_t rightCount;
  int32_t fwdCount;
  int32_t rotCount;
};

struct TempStruct {
  // holds temporary calculation variables
  int32_t oldLeftEncoder;
  int32_t oldRightEncoder;
  int32_t leftEncoder;
  int32_t rightEncoder;
//  int32_t encoderSum;
//  int32_t encoderDiff;
};

// global variables for tracking usage
extern volatile struct SpeedStruct LRSpeedStruct;
extern volatile struct DistStruct LRDistTotal;
extern volatile struct TempStruct LRTempStruct;

/***************************** ENCODER METHODS **********************************
 * Tracking for the encoders can be done through a simple ENC_Init() command.
 * Reading the timer ticks should be able to be done using the global structs above,
 * but it may also be read directly from the timer type def.
 *
 * The structs should be set to auto-update when the SysTickInit() function is called.
 * This would probably be the most useful way to set up the encoder interrupt. NVIC
 * can probably be used to configure the order in which the interrupt occurs...
 *******************************************************************************/
void END_Read();

void ENC_Reset();

void ENC_Init();

void ENC_SystTickInit();

void SysTick_Handler(void);

#ifdef __cplusplus
  }
#endif
#endif /* DUALENCODER_H_ */
