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
	mutex.blockedTasks.head = NULL;
	mutex.blockedTasks.tail = NULL;
	return mutex;
}


kReturnValue_t threads_mutexLock(volatile struct kLockStruct_t* mutex)
{
	kReturnValue_t exitcode = threads_semaphoreWait(mutex);
	return exitcode;
}

kReturnValue_t threads_mutexUnlock(volatile struct kLockStruct_t* mutex)
{
	kReturnValue_t exitcode = threads_semaphoreSignal(mutex);
	return exitcode;
}