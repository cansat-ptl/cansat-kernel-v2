/*
 * threads.h
 *
 * Created: 13.02.2020 19:37:27
 *  Author: Admin
 */ 

#ifndef THREADS_H_
#define THREADS_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

void kernel_exitCriticalSection();
void kernel_enterCriticalSection();

struct kLock_t kernel_createMutex();
uint8_t kernel_lockMutex(struct kLock_t* mutex);
uint8_t kernel_unlockMutex(struct kLock_t* mutex);

#endif /* THREADS_H_ */