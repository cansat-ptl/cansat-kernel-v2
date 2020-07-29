/*
 * fifo.c
 *
 * Created: 18.02.2020 21:54:55
 *  Author: Admin
 */

#include "ipc.h"
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include <kernel/memory.h>
#include <kernel/ipc.h>
#include <stdlib.h>

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
		fifo = (kFifoHandle_t)memory_heapAlloc(kFifoStructSize);

		if (fifo != NULL) {
			size_t totalSize = itemSize * itemsTotal;
			void* pointer = memory_heapAlloc(totalSize);

			if (pointer != NULL) {
				threads_fifoCreateStatic(fifo, pointer, itemSize, totalSize);
			}
			else {
				memory_heapFree((void*)fifo);
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