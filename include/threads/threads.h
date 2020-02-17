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

struct kLock_t kernel_createMutex();
uint8_t kernel_lockMutex(struct kLock_t* mutex);
uint8_t kernel_unlockMutex(struct kLock_t* mutex);

struct kLock_t kernel_createSemaphore(uint8_t resourceAmount);
uint8_t kernel_waitSemaphore(struct kLock_t* semaphore);
uint8_t kernel_signalSemaphore(struct kLock_t* semaphore);

void kernel_acquireSpinlock(kSpinlock_t* spinlock);
void kernel_releaseSpinlock(kSpinlock_t* spinlock);

#endif /* THREADS_H_ */