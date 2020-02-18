/*
 * fifo.c
 *
 * Created: 18.02.2020 21:54:55
 *  Author: Admin
 */ 

/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <threads/threads.h>

kFifo_t threads_fifoInit(char* pointer, uint8_t size)
{
	kFifo_t fifo;
	fifo.size = size;
	fifo.pointer = pointer;
	fifo.inputPosition = 0;
	fifo.outputPosition = 0;
	return fifo;
}

uint8_t threads_fifoAvailable(kFifo_t* fifo) 
{
	if (fifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if(fifo -> inputPosition == fifo -> outputPosition) {
		threads_endAtomicOperation(sreg);
		return 0;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 1;
	}
}

uint8_t threads_fifoWrite(kFifo_t* fifo, char data)
{
	if (fifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if (fifo -> inputPosition == ((fifo -> outputPosition - 1 + fifo -> size) % fifo -> size)) {
		threads_endAtomicOperation(sreg);
		return 0;
	}
	
	fifo -> pointer[fifo -> inputPosition] = data;
	fifo -> inputPosition = (fifo -> inputPosition + 1) % fifo -> size;
	
	threads_endAtomicOperation(sreg);
	return 1;
}

char threads_fifoRead(kFifo_t* fifo)
{
	if (fifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if(fifo -> inputPosition == fifo -> outputPosition) {
		threads_endAtomicOperation(sreg);
		return 0;
	}

	char data = fifo -> pointer[fifo -> outputPosition];
	fifo -> outputPosition = (fifo -> outputPosition + 1) % fifo -> size;
	
	threads_endAtomicOperation(sreg);
	return data;
}

char threads_fifoPeek(kFifo_t* fifo)
{
	if (fifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if(fifo -> inputPosition == fifo -> outputPosition) {
		threads_endAtomicOperation(sreg);
		return 0;
	}

	char data = fifo -> pointer[fifo -> outputPosition];
	
	threads_endAtomicOperation(sreg);
	return data;
}