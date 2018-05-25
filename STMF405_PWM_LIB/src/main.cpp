/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "PWMDriver.hpp"
#include "Timer.h"

void testMotorPWM(void);
void testBuzzerPWM(void);

int main(void) {
  TIM_Delay_Init();

  // proceed to either test (comment out unnecessary ones)
//  testMotorPWM();
  testBuzzerPWM();
}

void testMotorPWM(void) {
  // Test procedure for motor
  for(;;) {
    MotorDriver motorLeft(GPIOB, GPIO_Pin_9, TIM10, GPIOB, GPIO_Pin_8, TIM11);

    for(uint8_t i=0; i<50; i++) {
      motorLeft.driveForward(i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    for(int16_t i=50; i>=0; i--) {
      motorLeft.driveForward((uint8_t)i);
      TIM_Delay_Milli(100);
    }
    motorLeft.stallMotors();
    TIM_Delay_Micro(10);
    for(uint8_t i=0; i<50; i++) {
      motorLeft.driveBackward(i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    for(int16_t i=50; i>=0; i--) {
      motorLeft.driveBackward((uint8_t)i);
      TIM_Delay_Milli(100);
    }
    TIM_Delay_Micro(10);
    motorLeft.stallMotors();
    TIM_Delay_Milli(3000);
  }
}

void testBuzzerPWM() {
  // Test procedure for buzzer
  using NOTES = BuzzerDriver::Note;
  const NOTES NOTEARR[] = {NOTES::Cl, NOTES::D, NOTES::E, NOTES::F, NOTES::G,
                         NOTES::A, NOTES::B, NOTES::Ch};

  using KEYS = BuzzerDriver::Key;
  const KEYS KEYARR[] = {KEYS::C1, KEYS::C2, KEYS::C3, KEYS::C4, KEYS::C5,
                       KEYS::C6, KEYS::C7, KEYS::C8};

  // prepare buzzer
  BuzzerDriver buzz(GPIOA, GPIO_Pin_11, TIM1, TIM_Channel::_4);

  // permanent loop
//  for(;;) {
//    for(const auto& cNote : NOTEARR) {
//      for(const auto& cKey: KEYARR) {
//        buzz.playNote(cNote, cKey);
//        TIM_Delay_Milli(2000);
//      }
//    }

    for(;;) {
      buzz.playFrequency(500);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(1000);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(1500);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(2000);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(2500);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(3000);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(3500);
      TIM_Delay_Milli(1000);
      buzz.playFrequency(4000);
      TIM_Delay_Milli(1000);
    }
}
