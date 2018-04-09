/**
  ******************************************************************************
  * @file    HardwareSerial.h
  * @author  Kenmei
  * @brief   A somewhat quick implementation of a serial class for the STMF4
  * 		 using the SPL. It includes basic features for printing basic
  * 		 numbers and integers.
  *
  * 		 TODO: Implement serial interface for any given serial port
  * 		 TODO: Clean up header and .c files for serial class
  ******************************************************************************
*/

#ifndef __HARD_SERIAL_H
#define __HARD_SERIAL_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stm32f4xx.h>
#include <stdbool.h> // used for boolean values

/************** BEGIN RING BUFFER DECLARATIONS **************/
/***********************************************************
 * USER DEFINED CONSTANTS
 ***********************************************************/
#define _DEFAULT_RING_SIZE 	512
#define __PRECISION 		2

// Structure of a RingBuffer
typedef struct{
   uint16_t _head;
   uint16_t _tail;
   uint8_t _buffer[_DEFAULT_RING_SIZE];
} RingBuffer;

// Methods for RingBuffers
/**
 * Given an empty ring buffer object, initialize the elements inside
 */
void RingBuffer_init(RingBuffer* buf);

/**
 * Checks if the buffer is empty
 * @param buf - A pointer to the buffer object
 * @return True if the buffer is empty, false otherwise.
 */
bool RingBuffer_isEmpty(RingBuffer* buf);

/**
 * Checks if the buffer is full
 * @param buf - A pointer to the buffer object
 * @return True if the buffer is full, false otherwise.
 */
bool RingBuffer_isFull(RingBuffer* buf);

/**
 * Adds an element to the ring buffer, if possible.
 * @param buf - A pointer to the buffer object
 *        toAdd - The element to add to the buffer
 * @return Nothing
 */
bool RingBuffer_queue(RingBuffer* buf, uint8_t toAdd);

/**
 * Removes an element from the ring buffer.
 * @param buf - A pointer to the buffer object
 * @return The current element in the ring buffer pointed by the head
 */
uint8_t RingBuffer_dequeue(RingBuffer* buf);

/********************** END RING BUFFER ********************/
/***********************************************************/
/****************** BEGIN SERIAL INTERFACE *****************/

// Defines the two ring buffers needed for USART2
RingBuffer rx2Buf;
RingBuffer tx2Buf;

// Defines
typedef enum { __RX = 0, __TX = 1 } RTX_Type;

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
 * @param twrite - the integer to write to the stream
 * @return true if the transfer was successful, false otherwise
 */
bool HUSART_writeInt(USART_TypeDef* usart, int twrite);

/**
 * A method of writing float numbers to the serial port. Very, very shaky
 * implementation.
 * @param usart - the serial peripheral to send data from
 * @param twrite - the float number to write to the stream
 * @return true if the transfer was successful, false otherwise
 */
bool HUSART_writeFloat(USART_TypeDef* usart, double twrite);

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

#ifdef __cplusplus
  }
#endif

#endif /*************** END SERIAL INTERFACE **************/
