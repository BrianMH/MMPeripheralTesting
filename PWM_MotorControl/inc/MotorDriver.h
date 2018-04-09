/**
  ******************************************************************************
  * @file    HardwareSerial.h
  * @author  Kenmei
  * @brief   The motor driver library is basically a PWM library for usse with
  *          any pins. It will be generalized to two pins for now (but it may
  *          be generalized even further later)
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h" // timer library
#include "stm32f4xx_rcc.h" // rcc library
#include "stm32f4xx_gpio.h"   // gpio library

class PWMDriver {
public:
  // Initialize class based on Timer, PinGroup (i.e. A), PinNumber
  PWMDriver(TIM_TypeDef * tTim, GPIO_TypeDef * tPinGroup );

private:

};
