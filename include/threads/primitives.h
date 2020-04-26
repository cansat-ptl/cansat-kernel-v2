/*
 * primitives.h
 *
 * Created: 18.02.2020 19:43:17
 *  Author: Admin
 */ 


#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <types.h>

void threads_exitCriticalSection();
void threads_enterCriticalSection();
uint8_t threads_startAtomicOperation();
void threads_endAtomicOperation(uint8_t sreg);

void threads_notificationWait();
uint8_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

struct kLock_t threads_mutexInit();
uint8_t threads_mutexLock(struct kLock_t* mutex);
uint8_t threads_mutexUnlock(struct kLock_t* mutex);

struct kLock_t threads_semaphoreInit(uint8_t resourceAmount);
uint8_t threads_semaphoreWait(struct kLock_t* semaphore);
uint8_t threads_semaphoreSignal(struct kLock_t* semaphore);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* PRIMITIVES_H_ */