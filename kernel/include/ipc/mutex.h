/*
 * mutex.h
 *
 * Created: 16.06.2020 16:30:56
 *  Author: Admin
 */ 


#ifndef MUTEX_H_
#define MUTEX_H_

struct kLockStruct_t;
typedef volatile struct kLockStruct_t kMutex_t;
typedef volatile kMutex_t* kMutexHandle_t;

struct kLockStruct_t threads_mutexInit();
kReturnValue_t threads_mutexLock(volatile struct kLockStruct_t* mutex);
kReturnValue_t threads_mutexUnlock(volatile struct kLockStruct_t* mutex);

#endif /* MUTEX_H_ */