/*
 * fifo.c
 *
 * Created: 18.02.2020 21:54:55
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

static const size_t kFifoStructSize	= (sizeof(struct kIPCStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

kReturnValue_t threads_fifoCreateStatic(kFifoHandle_t fifo, char* pointer, size_t itemSize, size_t totalSize)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (fifo != NULL && pointer != NULL && itemSize != 0 && totalSize != 0) {
		fifo->itemSize = itemSize;
		fifo->size = totalSize;
		fifo->pointer = pointer;
		fifo->inputPosition = 0;
		fifo->outputPosition = 0;
		fifo->currentPosition = 0;
		exitcode = 0;
	}
	else {
		exitcode = ERR_NULLPTR;
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

kReturnValue_t threads_fifoWrite(kFifoHandle_t fifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (fifo != NULL) {
		if (threads_fifoFreeSpace(fifo)) {
			memcpy(fifo->pointer + fifo->inputPosition, item, fifo->itemSize);
			
			fifo->inputPosition += fifo->itemSize;
			
			if (fifo->inputPosition >= fifo->size) {
				fifo->inputPosition = 0;
			}
			
			fifo->currentPosition += fifo->itemSize;
			exitcode = 0;
		}
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

kReturnValue_t threads_fifoRead(kFifoHandle_t fifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (fifo != NULL) {
		if (threads_fifoAvailable(fifo)) {
			memcpy(item, fifo->pointer + fifo->outputPosition, fifo->itemSize);
			
			fifo->outputPosition += fifo->itemSize;
			
			if (fifo->outputPosition >= fifo->size) {
				fifo->outputPosition = 0;
			}
			
			fifo->currentPosition -= fifo->itemSize;
			exitcode = 0;
		} 
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

kReturnValue_t threads_fifoPeek(kFifoHandle_t fifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (fifo != NULL) {
		if (threads_fifoAvailable(fifo)) {
			memcpy(item, fifo->pointer + fifo->outputPosition, fifo->itemSize);
		}
	}
	else {
		exitcode = ERR_NULLPTR;
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
}

kReturnValue_t threads_lifoCreateStatic(kLifoHandle_t lifo, void* pointer, size_t itemSize, size_t totalSize)
{
	return threads_fifoCreateStatic(lifo, pointer, itemSize, totalSize);
}

kLifoHandle_t threads_lifoCreate(size_t itemSize, size_t itemsTotal)
{
	return threads_fifoCreate(itemSize, itemsTotal);
}

kReturnValue_t threads_lifoWrite(kFifoHandle_t lifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (lifo != NULL) {
		if (threads_lifoFreeSpace(lifo)) {
			memcpy(lifo->pointer + lifo->currentPosition, item, lifo->itemSize);
			
			lifo->currentPosition += lifo->itemSize;
			
			exitcode = 0;
		}
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

kReturnValue_t threads_lifoRead(kFifoHandle_t lifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (lifo != NULL) {
		if (threads_lifoAvailable(lifo)) {
			memcpy(item, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
			
			lifo->currentPosition -= lifo->itemSize;
			
			exitcode = 0;
		}
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

kReturnValue_t threads_lifoPeek(kFifoHandle_t lifo, void* item)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (lifo != NULL) {
		if (threads_fifoAvailable(lifo)) {
			memcpy(item, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
		}
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

size_t threads_lifoFreeSpace(kFifoHandle_t lifo)
{
	if (lifo->size - lifo->currentPosition >= lifo->itemSize)
	return lifo->size - lifo->currentPosition;
	else
	return 0;
}

size_t threads_lifoAvailable(kFifoHandle_t lifo)
{
	return lifo->currentPosition;
}