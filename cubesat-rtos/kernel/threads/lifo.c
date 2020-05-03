/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

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
	uint8_t exitcode = 0;
	if (lifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if (lifo -> currentPosition != 0) exitcode = 1;
		
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}

uint8_t threads_lifoWrite(kLifo_t* lifo, char data)
{
	uint8_t exitcode = 0;
	if (lifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if (lifo -> currentPosition < lifo -> size) {
			lifo -> currentPosition++;
			lifo -> pointer[lifo -> currentPosition] = data;
			exitcode = 1;
		}
		
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}

char threads_lifoRead(kLifo_t* lifo)
{
	char data = 0;
	if (lifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if (lifo -> currentPosition != 0) {
			data = lifo -> pointer[lifo -> currentPosition];
			lifo -> currentPosition--;
		}
		
		threads_endAtomicOperation(sreg);
	}
	return data;
}

char threads_lifoPeek(kLifo_t* lifo)
{
	char data = 0;
	if (lifo != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		if (lifo -> currentPosition != 0) {
			data = lifo -> pointer[lifo -> currentPosition];
		}
		
		threads_endAtomicOperation(sreg);
	}
	return data;
}