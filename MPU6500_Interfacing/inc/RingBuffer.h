/**
  ******************************************************************************
  * @file    RingBuffer.h
  * @author  Kenemi
  * @brief   A simple ring buffer made for the serial peripheral.
  *          The appropriate way to create a RingBuffer is to first declare the
  *          struct and then initialize it using the RingBuffer_init function.
  *          This file includes the most basic elements needed for the queue
  *          in a basic RingBuffer_xxxxx format:
  *            queue - add an element to the buffer
  *            dequeue - remove the current element
  *            isFull - checks if the buffer is full
  *            isEmpty - checks if the buffer is empty
  ******************************************************************************
*/

// Prevent recursive inclusion
#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

// Imported Libraries
#include <stdint.h>

// Defines
#define _DEFAULT_RING_SIZE 512

/**
 * NOTE: DO NOT COMPARE DIRECTLY WITH TRUE CONST!!! THERE IS NO GUARANTEE THAT
 *       IT IS 1
 */
#ifndef _BOOL
#define _BOOL
typedef enum { false=0, true=!false } bool;
#endif


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

#endif
