/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <threads/threads.h>

kLifo_t threads_lifoInit(char* pointer, uint8_t size)
{
	kLifo_t lifo;
	lifo.size = size;
	lifo.pointer = pointer;
	lifo.currentPosition = 0;
	return lifo;
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

char threads_lifoPeek(kLifo_t* lifo)
{
	if (lifo == NULL) return 0;
	
	uint8_t sreg = threads_startAtomicOperation();
	
	if (lifo -> currentPosition != 0) {
		char data = lifo -> pointer[lifo -> currentPosition];
		threads_endAtomicOperation(sreg);
		return data;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 0;
	}
}