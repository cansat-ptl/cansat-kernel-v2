/*
 * atomic.c
 *
 * Created: 18.02.2020 22:26:40
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>
#include <kernel/platform/platform.h>

kStatusRegister_t threads_startAtomicOperation()
{
	kStatusRegister_t sreg = platform_startAtomicOperation();
	return sreg;
}

void threads_endAtomicOperation(kStatusRegister_t sreg)
{
	platform_endAtomicOperation(sreg);
}