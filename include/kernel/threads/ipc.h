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

kFifo_t threads_fifoInit(char* pointer, uint8_t size);
uint8_t threads_fifoAvailable(kFifo_t* fifo);
uint8_t threads_fifoWrite(kFifo_t* fifo, char data);
char threads_fifoRead(kFifo_t* fifo);
char threads_fifoPeek(kFifo_t* fifo);

#endif /* IPC_H_ */