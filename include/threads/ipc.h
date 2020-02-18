/*
 * ipc.h
 *
 * Created: 18.02.2020 19:43:28
 *  Author: Admin
 */ 


#ifndef IPC_H_
#define IPC_H_

#include <types.h>

kLifo_t threads_lifoInit(char* pointer, uint8_t size);
uint8_t threads_lifoAvailable(kLifo_t* lifo);
uint8_t threads_lifoWrite(kLifo_t* queue, char data);
char threads_lifoRead(kLifo_t* queue);
uint8_t threads_lifoWriteMulti(kLifo_t* queue, char* data, uint8_t size);

#endif /* IPC_H_ */