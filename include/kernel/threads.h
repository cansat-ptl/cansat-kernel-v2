/*
 * sync.h
 *
 * Created: 21.06.2020 20:22:32
 *  Author: Admin
 */ 


#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>

struct kLockStruct_t;

typedef volatile uint8_t kSpinlock_t;

typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile struct kLockStruct_t kMutex_t;

typedef volatile struct kLockStruct_t* kMutexHandle_t;
typedef volatile struct kLockStruct_t* kSemaphoreHandle_t;
typedef volatile kSpinlock_t* kSpinlockHandle_t;

kMutexHandle_t threads_mutexCreate();
kReturnValue_t threads_mutexLock(volatile struct kLockStruct_t* mutex);
kReturnValue_t threads_mutexUnlock(volatile struct kLockStruct_t* mutex);

kSemaphoreHandle_t threads_semaphoreCreate(uint8_t resourceAmount);
kReturnValue_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore);
kReturnValue_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore);

uint16_t threads_startCriticalSection();
void threads_endCriticalSection();

kStatusRegister_t threads_startAtomicOperation();
void threads_endAtomicOperation(kStatusRegister_t sreg);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* SYNC_H_ */