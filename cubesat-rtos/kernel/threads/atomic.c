/*
 * atomic.c
 *
 * Created: 18.02.2020 22:26:40
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

uint8_t threads_startAtomicOperation()
{
	#if CFG_ALLOW_INTERRUPTS_IN_ATOMIC_SECTIONS == 0
		kStatusRegister_t sreg = platform_STATUS_REG;
		platform_DISABLE_INTERRUPTS();
		return sreg;
	#else
		return 0;
	#endif
}

void threads_endAtomicOperation(kStatusRegister_t sreg)
{
	#if CFG_ALLOW_INTERRUPTS_IN_ATOMIC_SECTIONS == 0
		platform_ENABLE_INTERRUPTS();
		platform_STATUS_REG = sreg;
	#endif
}