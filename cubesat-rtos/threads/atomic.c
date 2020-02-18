/*
 * atomic.c
 *
 * Created: 18.02.2020 22:26:40
 *  Author: Admin
 */ 

#include <threads/threads.h>

uint8_t threads_startAtomicOperation()
{
	threads_enterCriticalSection();
	#if CFG_NO_INTERRUPTS_IN_CRITICAL_SECTIONS == 1
		uint8_t sreg = platform_STATUS_REG;
		platform_DISABLE_INTERRUPTS();
		return sreg;
	#else
		return 0;
	#endif
}

void threads_endAtomicOperation(uint8_t sreg)
{
	threads_exitCriticalSection();
	#if CFG_NO_INTERRUPTS_IN_CRITICAL_SECTIONS == 1
		platform_ENABLE_INTERRUPTS();
		platform_STATUS_REG = sreg;
	#endif
}