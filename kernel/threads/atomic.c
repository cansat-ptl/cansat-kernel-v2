/*
 * atomic.c
 *
 * Created: 18.02.2020 22:26:40
 *  Author: Admin
 */ 

#include "threads.h"
#include "../platform/platform.h"
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include <kernel/threads.h>

kStatusRegister_t threads_startAtomicOperation()
{
	kStatusRegister_t sreg = platform_startAtomicOperation();
	return sreg;
}

void threads_endAtomicOperation(kStatusRegister_t sreg)
{
	platform_endAtomicOperation(sreg);
}