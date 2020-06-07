/*
 * primitives.h
 *
 * Created: 18.02.2020 19:43:17
 *  Author: Admin
 */ 


#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <kernel/types.h>

void threads_exitCriticalSection(); //Deprecated
void threads_enterCriticalSection(); //Deprecated

uint16_t threads_startCriticalSection();
void threads_endCriticalSection(uint16_t kflags);

kStatusRegister_t threads_startAtomicOperation();
void threads_endAtomicOperation(kStatusRegister_t sreg);

uint16_t threads_notificationWait();
uint8_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

struct kLockStruct_t threads_mutexInit();
uint8_t threads_mutexLock(volatile struct kLockStruct_t* mutex);
uint8_t threads_mutexUnlock(volatile struct kLockStruct_t* mutex);

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount);
uint8_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore);
uint8_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* PRIMITIVES_H_ */