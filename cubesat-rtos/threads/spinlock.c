/*
 * spinlock.c
 *
 * Created: 17.02.2020 22:11:30
 *  Author: Admin
 */ 

#include <threads/threads.h>

void threads_spinlockAcquire(kSpinlock_t* spinlock) 
{
	while(1) {
		uint8_t sreg = threads_startAtomicOperation();
		if(*spinlock == 0) {
			if(*spinlock == 0) {
				*spinlock = 1;
				threads_endAtomicOperation(sreg);
				return;
			}
		}
		threads_endAtomicOperation(sreg);
	}
}

void threads_spinlockRelease(kSpinlock_t* spinlock)
{
	uint8_t sreg = threads_startAtomicOperation();
	*spinlock = 0;
	threads_endAtomicOperation(sreg);
}