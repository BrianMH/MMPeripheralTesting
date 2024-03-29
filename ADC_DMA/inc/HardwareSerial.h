#ifndef __HARD_SERIAL_H
#define __HARD_SERIAL_H

#include <RingBuffer.h>
#include <stdint.h>
#include <stm32f4xx.h>

// Defines the two ring buffers needed for USART2
RingBuffer rx2Buf;
RingBuffer tx2Buf;

// Defines
typedef enum { __RX = 0, __TX = 1 } RTX_Type;

#ifndef _BOOL
#define _BOOL
typedef enum { false=0, true=!false } bool;
#endif

// Methods for the HardwareSerial class. By default, this class initializes the
// USART 2 interface. Extending it to others is just a matter of using defines.

/**
 * Initializes the specified usart/uart peripheral with the specified baud rate,
 * if possible.
 * @param usart - the usart peripheral to turn on
 * @param baud - the baud rate to use for the usart peripheral
 * @return Nothing
 */
void HUSART_begin(USART_TypeDef* usart, uint32_t baud);

/**
 * There are various ring buffers that may be needed for the various USARTs. This
 * method allows there to be a way to maintain generality in tx/rx methods.
 * @param usart - the usart peripheral being used
 * @param toRec - the ring buffer (tx/rx) to receive
 * @return the ring buffer requested
 */
RingBuffer* _HUSART_RET_BUF(USART_TypeDef* usart, RTX_Type toRec);

/**
 * Reads from the rxbuffer of the specified usart peripheral
 * @param usart - the serial peripheral to read from
 * @returns the data that was retrieved, if any
 */
uint8_t HUSART_read(USART_TypeDef* usart);

/**
 * Writes a single character to the USART peripheral. (Not very interesting,
 * but more of a starter function for the class.)
 * @param usart - the serial peripheral to send data from
 * @param twrite - the char to send
 * @return true if the transfer was successful; false otherwise
 */
bool HUSART_writeChar(USART_TypeDef* usart, char twrite);

/**
 * Writes a string that is '\0' delimited to the desired USART peripheral.
 * @param usart - the serial peripheral to send data from
 * @param twrite - a pointer to the buffer containing the stream
 * @return true if the transfer was successful; false otherwise.
 */
bool HUSART_writeString(USART_TypeDef* usart, char* twrite);

/**
 * A method of writing numbers to the serial port was needed, so this was
 * implemented. It is a very simple function that prints (sizeof int) integers.
 * @param usart - the serial peripheral to send data from
 * @param twrite - a pointer to the buffer containing the stream
 * @return true if the transfer was succesful, false otherwise
 */
bool HUSART_writeInt(USART_TypeDef* usart, int twrite);

/**
 * Reports if the USART peripheral has any data in the buffer.
 * @param usart - the serial peripheral to check incoming data from
 * @return true if there is data available to read; false otherwise
 */
bool HUSART_available(USART_TypeDef* usart);

/**
 * Flushes the incoming and leaving buffers.
 * @param usart - the serial peripheral to flush
 * @return nothing
 */
void HUSART_flush(USART_TypeDef* usart);

/**
 * Interrupt Service Routine that uses the NVIC
 *
 * NOTE: Feel free to set the priority and sub-priority of the handler by
 *       calling a combination of the following functions:
 *        NVIC_SetPriority(USART_TypeDef* usart, int priority);
 *        NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
 *        NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
 */
void USART2_IRQHandler(void);

#endif
