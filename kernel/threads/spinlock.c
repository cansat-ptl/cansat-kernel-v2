/*
 * spinlock.c
 *
 * Created: 17.02.2020 22:11:30
 *  Author: Admin
 */ 

#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include <kernel/threads.h>
#include "../platform/platform.h"

void threads_spinlockAcquire(kSpinlock_t* spinlock) 
{
	platform_spinlockAcquire(spinlock);
}

void threads_spinlockRelease(kSpinlock_t* spinlock)
{
	platform_spinlockRelease(spinlock);
}