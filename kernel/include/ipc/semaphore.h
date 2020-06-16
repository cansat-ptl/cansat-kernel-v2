/*
 * semaphore.h
 *
 * Created: 16.06.2020 16:31:06
 *  Author: Admin
 */ 


#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

struct kLockStruct_t;
typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile kSemaphore_t* kSemaphoreHandle_t;

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount);
kReturnValue_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore);
kReturnValue_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore);

#endif /* SEMAPHORE_H_ */