/*
 * ipc.h
 *
 * Created: 18.02.2020 19:43:28
 *  Author: Admin
 */ 


#ifndef IPC_H_
#define IPC_H_

#include <kernel/types.h>

kReturnValue_t threads_lifoCreateStatic(kLifoHandle_t lifo, void* pointer, size_t itemSize, size_t totalSize);
kLifoHandle_t threads_lifoCreate(size_t itemSize, size_t itemsTotal);
kReturnValue_t threads_lifoWrite(kFifoHandle_t lifo, void* item);
kReturnValue_t threads_lifoRead(kFifoHandle_t lifo, void* item);
kReturnValue_t threads_lifoPeek(kFifoHandle_t lifo, void* item);
size_t threads_lifoFreeSpace(kFifoHandle_t lifo);
size_t threads_lifoAvailable(kFifoHandle_t lifo);

kReturnValue_t threads_fifoCreateStatic(kFifoHandle_t fifo, char* pointer, size_t itemSize, size_t totalSize);
kFifoHandle_t threads_fifoCreate(size_t itemSize, size_t itemsTotal);

kReturnValue_t threads_fifoWrite(kFifoHandle_t fifo, void* item);
kReturnValue_t threads_fifoRead(kFifoHandle_t fifo, void* item);
kReturnValue_t threads_fifoPeek(kFifoHandle_t fifo, void* item);
size_t threads_fifoFreeSpace(kFifoHandle_t fifo);
size_t threads_fifoAvailable(kFifoHandle_t fifo);

#endif /* IPC_H_ */