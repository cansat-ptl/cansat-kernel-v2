/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include "threads.h"
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include <kernel/threads.h>

kMutexHandle_t threads_mutexCreate()
{
	volatile struct kLockStruct_t* returnValue = threads_semaphoreCreate(1);
	returnValue->type = KLOCK_MUTEX;
	return returnValue;
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