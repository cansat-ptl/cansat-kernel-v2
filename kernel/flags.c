/*
 * flags.c
 *
 * Created: 16.02.2020 0:24:01
 *  Author: Admin
 */

#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include "utils/utils.h"

#define kernel_DISABLE_CONTEXT_SWITCH() utils_CLEAR_BIT(_kflags, KFLAG_CSW_ALLOWED)
#define kernel_ENABLE_CONTEXT_SWITCH() utils_SET_BIT(_kflags, KFLAG_CSW_ALLOWED)

volatile uint16_t _kflags = 0;
volatile uint8_t _kTaskMgrFlags = 0;
static volatile uint8_t _kSystemStatus = 0;

void threads_enterCriticalSection() //Deprecated
{
	kernel_DISABLE_CONTEXT_SWITCH();
}

void threads_exitCriticalSection() //Deprecated
{
	kernel_ENABLE_CONTEXT_SWITCH();
}

void threads_startCriticalSection()
{
	kernel_DISABLE_CONTEXT_SWITCH();
}

void threads_endCriticalSection(uint16_t kflags)
{
	kernel_ENABLE_CONTEXT_SWITCH();
}

void kernel_setFlag(uint8_t flag, uint8_t value)
{
	utils_WRITE_BIT(_kflags, flag, value);
}

uint8_t kernel_checkFlag(uint8_t flag)
{
	uint8_t res = utils_CHECK_BIT(_kflags, flag);
	return res;
}

uint8_t kernel_getSystemStatus()
{
	return _kSystemStatus;
}

void kernel_setSystemStatus(uint8_t status)
{
	_kSystemStatus = status;
}
