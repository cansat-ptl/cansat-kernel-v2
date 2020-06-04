/*
 * queue.c
 *
 * Created: 18.02.2020 18:11:55
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

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

static const size_t kLifoStructSize	= (sizeof(struct kSystemIO_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

uint8_t threads_lifoCreateStatic(kLifoHandle_t lifo, void* pointer, size_t itemSize, size_t totalSize)
{
	return threads_fifoCreateStatic(lifo, pointer, itemSize, totalSize);
}

kLifoHandle_t threads_lifoCreate(size_t itemSize, size_t itemsTotal)
{
	return threads_fifoCreate(itemSize, itemsTotal);
}

uint8_t threads_lifoWrite(kFifoHandle_t lifo, void* item)
{
	uint8_t exitcode = 1;
	if (lifo != NULL) {
		if (threads_lifoFreeSpace(lifo)) {
			memcpy(lifo->pointer + lifo->currentPosition, item, lifo->itemSize);
			
			lifo->currentPosition += lifo->itemSize;
			
			exitcode = 0;
		}
	}
	return exitcode;
}

uint8_t threads_lifoRead(kFifoHandle_t lifo, void* item)
{
	uint8_t exitcode = 1;
	if (lifo != NULL) {
		if (threads_lifoAvailable(lifo)) {
			memcpy(item, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
			
			lifo->currentPosition -= lifo->itemSize;
			
			exitcode = 0;
		} 
	}
	return exitcode;
}

uint8_t threads_lifoPeek(kFifoHandle_t lifo, void* item)
{
	uint8_t exitcode = 1;
	if (lifo != NULL) {
		if (threads_fifoAvailable(lifo)) {
			memcpy(item, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
		}
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
