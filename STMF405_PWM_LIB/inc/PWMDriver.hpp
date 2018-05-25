/*
 * MotorPWM.hpp
 *
 *  Created on: Apr 23, 2018
 *      Author: brian
 */

#ifndef PWMDRIVER_HPP_
#define PWMDRIVER_HPP_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

// used for determining channel of timer being used...
enum class TIM_Channel{_1, _2, _3, _4};

class PWMDriver {
public:
  // Use this to tailor frequency changes MUCH more carefully
  struct FreqAdjust {
    uint16_t clockPrescaler;
    uint32_t timerPeriod;
  };

private:
  TIM_TypeDef* pwmTimer;
  TIM_Channel pwmChannel;

public:
  PWMDriver(GPIO_TypeDef* pinGroup, uint16_t gpioPin, TIM_TypeDef* pwmTimer,
            TIM_Channel tChan, uint32_t frequency, FreqAdjust* adjuster = nullptr);

  void changeDutyCycle(const uint8_t dutyCycle);
  void changeFrequency(const uint32_t freq);
  void changeFrequency(const FreqAdjust& adjustment);
};

class MotorDriver {
protected:
  PWMDriver forward;
  PWMDriver backward;
  static constexpr uint32_t DEFAULT_FREQUENCY = 1000; // default pwm freq is 1kHz

public:
  MotorDriver(GPIO_TypeDef* pinGroupForward, uint16_t gpioPinForward,
           TIM_TypeDef* pwmTimerForward,
           GPIO_TypeDef* pinGroupBackward, uint32_t gpioPinBackward,
           TIM_TypeDef* pwmTimerBackward,
           const TIM_Channel pwmTimerChanForward = TIM_Channel::_1,
           const TIM_Channel pwmTimerChanBackward = TIM_Channel::_1);
  MotorDriver(const MotorDriver&) = delete;     // prevent copy ctor
                                                // (use references)

  void driveForward(const uint8_t dutyCycle);
  void driveBackward(const uint8_t dutyCycle);
  void stallMotors(void);
};

class BuzzerDriver{
private:
  // default parameter (makes no difference)
  static constexpr uint32_t INIT_FREQUENCY = 1000;

protected:
  // Default PWM must always have CCRx of 50% of period!
  PWMDriver buzzer;
//  bool buzzerOn;

public:
  // Used for simply mentioning key and note in play function
  enum class Key : uint8_t {
    C1 = 128,  // octave 1
    C2 = 64,   // octave ...
    C3 = 32,
    C4 = 16,
    C5 = 8,
    C6 = 4,
    C7 = 2,
    C8 = 1};
  enum class Note : uint32_t{
    Ch = 8372, // defined from octave 8!
    B = 7902,
    A = 7040,
    G = 6272,
    F = 5588,
    E = 5274,
    D = 4699,
    Cl = 4186};

  BuzzerDriver(GPIO_TypeDef* pinGroup, uint16_t gpioPin, TIM_TypeDef* pwmTimer, const TIM_Channel pwmChan);
  BuzzerDriver(const BuzzerDriver&) = delete;   // please don't make copies. this class is heavy enough...

  // plays a note. Default note value is tuned to C4
  void playNote(const Note nToPlay, const Key kToPlay = Key::C4);
  // plays a note given the frequency
  void playFrequency(const uint32_t freq);
  // stops any note from currently playing
  void stopNote(void);
};

#endif /* PWMDRIVER_HPP_ */
