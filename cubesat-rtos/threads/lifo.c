/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <kernel.h>

kLifo_t kernel_lifoInit(char* pointer, uint8_t size)
{
	kLifo_t queue;
	queue.size = size;
	queue.pointer = pointer;
	queue.currentPosition = 0;
	return queue;
}

uint8_t kernel_lifoAvailable(kLifo_t* lifo) 
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = kernel_startAtomicOperation();
	if (lifo -> currentPosition != 0) {
		kernel_endAtomicOperation(sreg);
		return 1;
	}
	else {
		kernel_endAtomicOperation(sreg);
		return 0;
	}
	
}

uint8_t kernel_lifoWrite(kLifo_t* lifo, char data)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = kernel_startAtomicOperation();
		
	if (lifo -> currentPosition < lifo -> size) {
		lifo -> currentPosition++;
		lifo -> pointer[lifo -> currentPosition] = data;
		kernel_endAtomicOperation(sreg);
		return 1;
	}
	else {
		kernel_endAtomicOperation(sreg);
		return 0;
	}
}

uint8_t kernel_lifoWriteMulti(kLifo_t* lifo, char* data, uint8_t size)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = kernel_startAtomicOperation();
	
	uint8_t dataWritten, result = 0;
	
	for (dataWritten = 0; dataWritten < size && result; dataWritten++) {
		result = kernel_lifoWrite(lifo, data[dataWritten]);
	}
	
	kernel_endAtomicOperation(sreg);
	return dataWritten;
}

char kernel_lifoRead(kLifo_t* lifo)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = kernel_startAtomicOperation();
	
	if (lifo -> currentPosition != 0) {
		char data = lifo -> pointer[lifo -> currentPosition];
		lifo -> currentPosition--;
		kernel_endAtomicOperation(sreg);
		return data;
	}
	else {
		kernel_endAtomicOperation(sreg);
		return 0;
	}
}