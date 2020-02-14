/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include <kernel.h>

struct kLock_t kernel_createMutex()  //TODO: this function is a stub. You can help by improving it
{
	struct kLock_t mutex;
	mutex.type = KLOCK_MUTEX;
	mutex.lockCount = 0;
	return mutex;
}


uint8_t kernel_lockMutex(struct kLock_t* mutex)
{
	
}

uint8_t kernel_unlockMutex(struct kLock_t* mutex)
{
	
}