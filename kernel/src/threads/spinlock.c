/*
 * spinlock.c
 *
 * Created: 17.02.2020 22:11:30
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>
#include <kernel/platform/platform.h>

void threads_spinlockAcquire(kSpinlock_t* spinlock) 
{
	platform_spinlockAcquire(spinlock);
}

void threads_spinlockRelease(kSpinlock_t* spinlock)
{
	platform_spinlockRelease(spinlock);
}