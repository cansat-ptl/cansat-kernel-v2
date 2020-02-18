/*
 * spinlock.c
 *
 * Created: 17.02.2020 22:11:30
 *  Author: Admin
 */ 

#include <kernel.h>

void kernel_acquireSpinlock(kSpinlock_t* spinlock) 
{
	while(1) {
		uint8_t sreg = kernel_startAtomicOperation();
		if(*spinlock == 0) {
			if(*spinlock == 0) {
				*spinlock = 1;
				kernel_endAtomicOperation(sreg);
				return;
			}
		}
		kernel_endAtomicOperation(sreg);
	}
}

void kernel_releaseSpinlock(kSpinlock_t* spinlock)
{
	uint8_t sreg = kernel_startAtomicOperation();
	*spinlock = 0;
	kernel_endAtomicOperation(sreg);
}