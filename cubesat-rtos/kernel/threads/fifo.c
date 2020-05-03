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

#include <kernel/threads/threads.h>

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
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		if(fifo -> inputPosition == fifo -> outputPosition) {
			exitcode = 0;
		}
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}

uint8_t threads_fifoWrite(kFifo_t* fifo, char data)
{
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if (fifo -> inputPosition == ((fifo -> outputPosition - 1 + fifo -> size) % fifo -> size)) {
			exitcode = 0;
		}
		else {
			fifo -> pointer[fifo -> inputPosition] = data;
			fifo -> inputPosition = (fifo -> inputPosition + 1) % fifo -> size;
		}
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}

char threads_fifoRead(kFifo_t* fifo)
{
	char data = 0;
	if (fifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if(!(fifo -> inputPosition == fifo -> outputPosition)) {
			data = fifo -> pointer[fifo -> outputPosition];
			fifo -> outputPosition = (fifo -> outputPosition + 1) % fifo -> size;
		}
		
		threads_endAtomicOperation(sreg);
	}
	return data;
}

char threads_fifoPeek(kFifo_t* fifo)
{
	char data = 0;
	if (fifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if(!(fifo -> inputPosition == fifo -> outputPosition)) data = fifo -> pointer[fifo -> outputPosition];
		
		threads_endAtomicOperation(sreg);
	}
	return data;
}