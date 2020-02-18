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
uint8_t kernel_startAtomicOperation();
void kernel_endAtomicOperation(uint8_t sreg);

struct kLock_t kernel_mutexInit();
uint8_t kernel_mutexLock(struct kLock_t* mutex);
uint8_t kernel_mutexUnlock(struct kLock_t* mutex);

struct kLock_t kernel_semaphoreInit(uint8_t resourceAmount);
uint8_t kernel_semaphoreWait(struct kLock_t* semaphore);
uint8_t kernel_semaphoreSignal(struct kLock_t* semaphore);

void kernel_spinlockAcquire(kSpinlock_t* spinlock);
void kernel_spinlockRelease(kSpinlock_t* spinlock);

kLifo_t kernel_lifoInit(char* pointer, uint8_t size);
uint8_t kernel_lifoAvailable(kLifo_t* lifo);
uint8_t kernel_lifoWrite(kLifo_t* queue, char data);
char kernel_lifoRead(kLifo_t* queue);
uint8_t kernel_lifoWriteMulti(kLifo_t* queue, char* data, uint8_t size);

#endif /* THREADS_H_ */