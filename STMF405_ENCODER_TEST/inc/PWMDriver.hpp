/**
  ******************************************************************************
  * @file    PWMDriver.hpp
  * @author  Kenmei
  * @brief   General PWM driver implementation. It includes an implementation
  *          unique to the motors and the buzzer.
  *
  *          TODO: Include frequency calculation in the PWMDriver class.
  *          TODO: Finish up buzzer driver class.
  ******************************************************************************
*/

#ifndef PWMDRIVER_HPP_
#define PWMDRIVER_HPP_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

enum class TIM_Channel{One, Two, Three, Four};

class PWMDriver {
public:
  // Use this to tailor frequency changes MUCH more carefully
  struct FreqAdjust {
    uint16_t clockPrescaler;
    uint32_t timerPeriod;
  };

private:
  TIM_TypeDef* pwmTimer;

public:
  PWMDriver(GPIO_TypeDef* pinGroup, uint16_t gpioPin, TIM_TypeDef* pwmTimer,
            uint32_t frequency, FreqAdjust* adjuster = nullptr);

  void changeDutyCycle(const uint8_t dutyCycle);
  void changeFrequency(const uint32_t freq);
  void changeFrequency(const FreqAdjust& adjustment);
};

class MotorDriver {
protected:
  PWMDriver forward;
  PWMDriver backward;
  static constexpr uint32_t DEFAULT_FREQUENCY = 1000;

public:
  MotorDriver(GPIO_TypeDef* pinGroupForward, uint16_t gpioPinForward,
           TIM_TypeDef* pwmTimerForward,
           GPIO_TypeDef* pinGroupBackcward, uint32_t gpioPinBackward,
           TIM_TypeDef* pwmTimerBackward);
  MotorDriver(const MotorDriver&) = delete;     // prevent copy ctor
                                                // (use references)

  void driveForward(const uint8_t dutyCycle);
  void driveBackward(const uint8_t dutyCycle);
  void stallMotors(void);
};

class BuzzerDriver : public PWMDriver {
  // Buzzer unique methods would go in here. Among them should be a way
  // of playing a melody given some data structure (likely a vector).
};

#endif /* PWMDRIVER_HPP_ */
