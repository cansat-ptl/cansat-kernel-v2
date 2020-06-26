/*
 * sync.h
 *
 * Created: 21.06.2020 20:22:32
 *  Author: Admin
 */ 


#ifndef SYNC_H_
#define SYNC_H_

#include <ktypes.h>

struct kLockStruct_t threads_mutexInit();
kReturnValue_t threads_mutexLock(volatile struct kLockStruct_t* mutex);
kReturnValue_t threads_mutexUnlock(volatile struct kLockStruct_t* mutex);

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount);
kReturnValue_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore);
kReturnValue_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore);

uint16_t tasks_notificationWait();
kReturnValue_t tasks_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

uint16_t threads_startCriticalSection();
void threads_endCriticalSection(uint16_t kflags);

kStatusRegister_t threads_startAtomicOperation();
void threads_endAtomicOperation(kStatusRegister_t sreg);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* SYNC_H_ */