/*
 * ipc.h
 *
 * Created: 18.02.2020 19:43:28
 *  Author: Admin
 */ 


#ifndef IPC_H_
#define IPC_H_

#include <kernel/types.h>

kLifo_t threads_lifoInit(char* pointer, uint8_t size);
uint8_t threads_lifoAvailable(kLifo_t* lifo);
uint8_t threads_lifoWrite(kLifo_t* queue, char data);
char threads_lifoRead(kLifo_t* queue);
char threads_lifoPeek(kLifo_t* queue);

uint8_t threads_fifoCreateStatic(kFifoHandle_t fifo, void* pointer, size_t itemSize, size_t totalSize);
kFifoHandle_t threads_fifoCreate(size_t itemSize, size_t itemsTotal);
uint8_t threads_fifoWrite(kFifoHandle_t fifo, void* item);
uint8_t threads_fifoRead(kFifoHandle_t fifo, void* item);
uint8_t threads_fifoPeek(kFifoHandle_t fifo, void* item);
size_t threads_fifoFreeSpace(kFifoHandle_t fifo);
size_t threads_fifoAvailable(kFifoHandle_t fifo);

#endif /* IPC_H_ */