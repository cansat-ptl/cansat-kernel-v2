/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

struct kLockStruct_t threads_mutexInit()
{
	struct kLockStruct_t mutex;
	mutex.type = KLOCK_MUTEX;
	mutex.lockCount = 1;
	return mutex;
}


uint8_t threads_mutexLock(struct kLockStruct_t* mutex)
{
	uint8_t exitcode = threads_semaphoreWait(mutex);
	return exitcode;
}

uint8_t threads_mutexUnlock(struct kLockStruct_t* mutex)
{
	uint8_t exitcode = threads_semaphoreSignal(mutex);
	return exitcode;
}