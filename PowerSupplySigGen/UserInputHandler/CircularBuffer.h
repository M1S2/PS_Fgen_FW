/*
 * CircularBuffer.h
 *
 * Created: 08.09.2020 20:08:37
 *  Author: V17
 * see: https://www.avrfreaks.net/forum/tut-c-regular-flow-data-using-producer-consumer
 */ 

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <avr/io.h>
#include "UserInputDataTypes.h"

#define NELEMS(x) (sizeof(x)/sizeof((x)[0]))

template <class T>
class CircularBuffer
{
	private:
		T queue[10];
		uint8_t head;
		uint8_t tail;
		
	public:
		
		// Returns 1 if the queue is empty, 0 otherwise
		uint8_t empty()
		{
			return (head == tail);	
		}
		
		// Returns 1 if the queue is full, 0 otherwise
		uint8_t full()
		{
			return (head == (tail + 1) % NELEMS(queue));
		}

		// enqueue() should never be called on a full queue
		void enqueue(T* data)
		{			
			queue[tail] = *data;
			tail = (tail + 1) % NELEMS(queue);
		}

		// dequeue() should never be called on an empty queue
		T* dequeue()
		{			
			T* data = &queue[head];
			head = (head + 1) % NELEMS(queue);
			return data;
		}
};

#endif /* CIRCULARBUFFER_H_ */