/*
 * flags.c
 *
 * Created: 16.02.2020 0:24:01
 *  Author: Admin
 */ 
#include <kernel.h>

#define kernel_DISABLE_CONTEXT_SWITCH() hal_CLEAR_BIT(_kflags, KFLAG_CSW_ALLOWED)
#define kernel_ENABLE_CONTEXT_SWITCH() hal_SET_BIT(_kflags, KFLAG_CSW_ALLOWED)

volatile uint16_t _kflags = 0;
volatile uint8_t _kTaskMgrFlags = 0;

void kernel_enterCriticalSection()
{
	kernel_DISABLE_CONTEXT_SWITCH();
}

void kernel_exitCriticalSection()
{
	kernel_ENABLE_CONTEXT_SWITCH();
}

uint8_t kernel_startAtomicOperation()
{
	kernel_enterCriticalSection();
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	return sreg;
}

void kernel_endAtomicOperation(uint8_t sreg)
{
	kernel_exitCriticalSection();
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void kernel_setFlag(uint8_t flag, uint8_t value)
{
	platform_DISABLE_INTERRUPTS();
	hal_WRITE_BIT(_kflags, flag, value);
	platform_ENABLE_INTERRUPTS();
}

uint8_t kernel_checkFlag(uint8_t flag)
{
	platform_DISABLE_INTERRUPTS();
	uint8_t res = hal_CHECK_BIT(_kflags, flag);
	platform_ENABLE_INTERRUPTS();
	return res;
}