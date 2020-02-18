/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <threads/threads.h>

kLifo_t threads_lifoInit(char* pointer, uint8_t size)
{
	kLifo_t queue;
	queue.size = size;
	queue.pointer = pointer;
	queue.currentPosition = 0;
	return queue;
}

uint8_t threads_lifoAvailable(kLifo_t* lifo) 
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	if (lifo -> currentPosition != 0) {
		threads_endAtomicOperation(sreg);
		return 1;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 0;
	}
	
}

uint8_t threads_lifoWrite(kLifo_t* lifo, char data)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
		
	if (lifo -> currentPosition < lifo -> size) {
		lifo -> currentPosition++;
		lifo -> pointer[lifo -> currentPosition] = data;
		threads_endAtomicOperation(sreg);
		return 1;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 0;
	}
}

uint8_t threads_lifoWriteMulti(kLifo_t* lifo, char* data, uint8_t size)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	uint8_t dataWritten, result = 0;
	
	for (dataWritten = 0; dataWritten < size && result; dataWritten++) {
		result = threads_lifoWrite(lifo, data[dataWritten]);
	}
	
	threads_endAtomicOperation(sreg);
	return dataWritten;
}

char threads_lifoRead(kLifo_t* lifo)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if (lifo -> currentPosition != 0) {
		char data = lifo -> pointer[lifo -> currentPosition];
		lifo -> currentPosition--;
		threads_endAtomicOperation(sreg);
		return data;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 0;
	}
}