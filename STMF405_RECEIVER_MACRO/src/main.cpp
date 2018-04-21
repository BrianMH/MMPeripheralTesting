/**
  ******************************************************************************
  * @file    main.c
  * @author  Kenmei
  * @brief   Simulates a typical analog data collection loop.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "EmitterMacros.h"
#include "ReceiverMacros.h"
#include "Timer.h"
#include "HardwareSerial.h"

void readSensorValues();

int main(void)
{
  // initialize components
  HUSART_begin(USART3, 9600);
  TIM_Delay_Init();
  EMIT_Init();
  REC_Init();

  for(;;) {
    readSensorValues();
    TIM_Delay_Milli(100);
  }
}

//typical value reading formation
void readSensorValues() {
  int valueRead = 0;
  int valueReadAlt = 0;

  IR_FRONT_ON;
  TIM_Delay_Micro(60);
  valueRead = (int)READ_FRONT_REC;
  IR_FRONT_OFF;
  HUSART_writeString(USART3, (char*)"Front: ");
  HUSART_writeInt(USART3, valueRead);

  IR_FLEFT_ON;
  TIM_Delay_Micro(60);
  valueRead = (int)READ_FLEFT_REC;
  valueReadAlt = (int)READ_BLEFT_REC;
  IR_FLEFT_OFF;
  HUSART_writeString(USART3, (char*)"\t\tFront Left:");
  HUSART_writeInt(USART3, valueRead);
  HUSART_writeString(USART3, (char*)",");
  HUSART_writeInt(USART3, valueReadAlt);

  IR_FRIGHT_ON;
  TIM_Delay_Micro(60);
  valueRead = (int)READ_FRIGHT_REC;
  valueReadAlt = (int)READ_BRIGHT_REC;
  IR_FRIGHT_OFF;
  HUSART_writeString(USART3, (char*)"\t\tFront Right:");
  HUSART_writeInt(USART3, valueRead);
  HUSART_writeString(USART3, (char*)",");
  HUSART_writeInt(USART3, valueReadAlt);

  IR_SIDES_ON;
  TIM_Delay_Micro(60);
  valueRead = (int) READ_LEFT_REC;
  valueReadAlt = (int) READ_RIGHT_REC;
  IR_SIDES_OFF;
  HUSART_writeString(USART3, (char*)"\t\tLeft: ");
  HUSART_writeInt(USART3, valueRead);
  HUSART_writeString(USART3, (char*)"\t\tRight: ");
  HUSART_writeInt(USART3, valueReadAlt);
}
