#include <RingBuffer.h>

// More of an insurance that values are not garbage
void RingBuffer_init(RingBuffer* buf) {
   buf->_head = 0;
   buf->_tail = 0;
   for(unsigned short ind = 0; ind < _DEFAULT_RING_SIZE; ind++) {
      buf->_buffer[ind] = 0;
   }
}

bool RingBuffer_isEmpty(RingBuffer* buf) {
   return buf->_head==buf->_tail;
}

bool RingBuffer_isFull(RingBuffer* buf) {
   return (((buf->_head)+1) % _DEFAULT_RING_SIZE)==buf->_tail;
}

bool RingBuffer_queue(RingBuffer* buf, uint8_t toAdd) {
   if(RingBuffer_isFull(buf)) // buffer is full; do nothing
      return false;

   buf->_buffer[buf->_head] = toAdd;
   if(++(buf->_head) >= _DEFAULT_RING_SIZE)
      buf->_head = 0;
   return true;
}

uint8_t RingBuffer_dequeue(RingBuffer* buf) {
   if(RingBuffer_isEmpty(buf)) // buffer is empty; nothing to do
      return 0;

   uint8_t ret = buf->_buffer[buf->_tail];
   if(++(buf->_tail) >= _DEFAULT_RING_SIZE)
      buf->_tail = 0;
   return ret;
}
