/*
 * threads.h
 *
 * Created: 13.02.2020 19:37:27
 *  Author: Admin
 */ 

#ifndef THREADS_H_
#define THREADS_H_

#include <types.h>

struct kLock_t kernel_createMutex();
uint8_t kernel_lockMutex(struct kLock_t* mutex);
uint8_t kernel_unlockMutex(struct kLock_t* mutex);

#endif /* THREADS_H_ */