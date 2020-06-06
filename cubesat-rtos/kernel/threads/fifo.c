/*
 * fifo.c
 *
 * Created: 18.02.2020 21:54:55
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

static const size_t kFifoStructSize	= (sizeof(struct kIPCStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

/*static void threads_fifoCopyFrom(kFifoHandle_t fifo, void * item)
{
	memcpy(item, fifo->pointer + fifo->outputPosition, fifo->itemSize);
	
	fifo->outputPosition += fifo->itemSize;
	
	if (fifo->outputPosition >= fifo->size) {
		fifo->outputPosition = 0;
	}
}

static void threads_fifoCopyTo(kFifoHandle_t fifo, void *item)
{
	memcpy(fifo->pointer + fifo->inputPosition, item, fifo->itemSize);
	
	fifo->inputPosition += fifo->itemSize;
	
	if (fifo->inputPosition >= fifo->size) {
		fifo->inputPosition = 0;
	}
}*/

uint8_t threads_fifoCreateStatic(kFifoHandle_t fifo, void* pointer, size_t itemSize, size_t totalSize)
{
	uint8_t exitcode = 1;
	if (fifo != NULL && pointer != NULL && itemSize != 0 && totalSize != 0) {
		fifo->itemSize = itemSize;
		fifo->size = totalSize;
		fifo->pointer = pointer;
		fifo->inputPosition = 0;
		fifo->outputPosition = 0;
		fifo->currentPosition = 0;
		exitcode = 0;
	}
	return exitcode;
}

kFifoHandle_t threads_fifoCreate(size_t itemSize, size_t itemsTotal)
{
	kFifoHandle_t fifo = NULL;
	
	if (itemsTotal > 0 && itemSize > 0) {
		fifo = (kFifoHandle_t)memmgr_heapAlloc(kFifoStructSize);
		
		if (fifo != NULL) {
			size_t totalSize = itemSize * itemsTotal;
			void* pointer = memmgr_heapAlloc(totalSize);
			
			if (pointer != NULL) {
				threads_fifoCreateStatic(fifo, pointer, itemSize, totalSize);
			}
			else {
				memmgr_heapFree((void*)fifo);
				fifo = NULL;
			}
		}
	}
	return fifo;
}
/*
uint8_t threads_fifoWrite(kFifoHandle_t fifo, void* item)
{
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		if (threads_fifoFreeSpace(fifo)) {
			threads_fifoCopyTo(fifo, item);
			
			fifo->currentPosition += fifo->itemSize;
			exitcode = 0;
		}
	}
	return exitcode;
}

uint8_t threads_fifoRead(kFifoHandle_t fifo, void* item)
{
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		if (threads_fifoAvailable(fifo)) {
			threads_fifoCopyFrom(fifo, item);
			
			fifo->currentPosition -= fifo->itemSize;
			exitcode = 0;
		} 
	}
	return exitcode;
}

uint8_t threads_fifoPeek(kFifoHandle_t fifo, void* item)
{
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		if (threads_fifoAvailable(fifo)) {
			memcpy(item, fifo->pointer + fifo->outputPosition, fifo->itemSize);
		}
	}
	return exitcode;
}

size_t threads_fifoFreeSpace(kFifoHandle_t fifo)
{
	if (fifo->currentPosition >= fifo->size)
		return 0;
	else
		return 1;
}


size_t threads_fifoAvailable(kFifoHandle_t fifo)
{
	return fifo->currentPosition;
}*/

uint8_t threads_fifoAvailable(kFifo_t* fifo)
{
	uint8_t exitcode = 1;
	if (fifo != NULL) {
		kStatusRegister_t sreg = threads_startAtomicOperation();
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
		kStatusRegister_t sreg = threads_startAtomicOperation();
		
		if (fifo -> inputPosition == ((fifo -> outputPosition - 1 + fifo -> size) % fifo -> size)) {
			exitcode = 0;
		}
		else {
			((char*)fifo -> pointer)[fifo -> inputPosition] = data;
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
		kStatusRegister_t sreg = threads_startAtomicOperation();
		
		if(!(fifo -> inputPosition == fifo -> outputPosition)) {
			data = ((char*)fifo -> pointer)[fifo -> outputPosition];
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
		kStatusRegister_t sreg = threads_startAtomicOperation();
		
		if(!(fifo -> inputPosition == fifo -> outputPosition)) data = ((char*)fifo -> pointer)[fifo -> outputPosition];
		
		threads_endAtomicOperation(sreg);
	}
	return data;
}
