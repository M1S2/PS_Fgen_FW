/**
 * @file	CircularBuffer.h
 * @date	08.09.2020 20:08:37
 * @author	Markus Scheich
 * @brief	Containing a class implementing a circular buffer (for queue functionality).
 * @see		https://www.avrfreaks.net/forum/tut-c-regular-flow-data-using-producer-consumer
 */ 

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <avr/io.h>
#include <stddef.h>

/**
 * Class that is implementing a circular buffer (for queue functionality).
 * @tparam T Type of the buffer elements
 * @tparam MaxElements Maximum number of elements the buffer can hold
 */
template <class T, size_t MaxElements>
class CircularBuffer
{
	private:
		T queue[MaxElements];				/**< Array holding the circular buffer data */
		volatile uint16_t head;				/**< Head index of the circular buffer */
		volatile uint16_t tail;				/**< Tail index of the circular buffer */
		
	public:
		
		/**
		 * Check if the circular buffer is empty.
		 * @return Returns 1 if the queue is empty, 0 otherwise
		 */
		inline uint8_t empty()
		{
			return (head == tail);	
		}
		
		/**
		 * Check if the circular buffer is full.
		 * @return Returns 1 if the queue is full, 0 otherwise
		 */
		inline uint8_t full()
		{
			return (head == (tail + 1) % MaxElements);
		}

		/**
		 * Enqueue the data at the given pointer into the queue.
		 * enqueue() should never be called on a full queue.
		 * @param data Pointer to the data that is enqueued.
		 */
		inline void enqueue(T* data)
		{			
			queue[tail] = *data;
			tail = (tail + 1) % MaxElements;
		}

		/**
		 * Dequeue data from the queue and return a pointer to the data
		 * dequeue() should never be called on an empty queue.
		 * @return Pointer to the data element in the queue.
		 */
		inline T* dequeue()
		{			
			T* data = &queue[head];
			head = (head + 1) % MaxElements;
			return data;
		}
};

#endif /* CIRCULARBUFFER_H_ */