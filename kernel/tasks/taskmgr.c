/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */

#include "scheduler.h"
#include "tasks.h"
#include "taskutils.h"
#include "../kernel.h"
#include "../platform/platform.h"
#include "../memory/memory.h"
#include "../memory/heap.h"
#include <kernel/ktypes.h>
#include <kdebug/debug.h>
#include <kernel/kernel_config.h>
#include <stdint.h>

kTaskHandle_t kCurrentTask;
static kTaskHandle_t kNextTask;
volatile uint64_t __e_time = 0;
extern volatile uint16_t _kflags;
static volatile uint8_t kInterruptDepth = 0;

extern volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
extern kStackPtr_t kStackPointer;

uint64_t kernel_getUptime()
{
	return __e_time; //TODO: move this somewhere else 
}

static void tasks_switchContext()
{
	#if CFG_MEMORY_PROTECTION_MODE == 1
		if (tasks_checkStackBounds(kCurrentTask)) {
			kernel_stackCorruptionHook(kCurrentTask);
		}
	#endif

	#if CFG_MEMORY_PROTECTION_MODE == 2
		#if CFG_STACK_GROWTH_DIRECTION == 0
			if (memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin), CFG_STACK_SAFETY_MARGIN)) {
				kernel_stackCorruptionHook(kCurrentTask);
			}
		#else
			if (memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
				kernel_stackCorruptionHook(kCurrentTask);
			}
		#endif
	#endif

	#if CFG_MEMORY_PROTECTION_MODE == 3
		#if CFG_STACK_GROWTH_DIRECTION == 0
			if (tasks_checkStackBounds(kCurrentTask) || memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin), CFG_STACK_SAFETY_MARGIN)) {
				kernel_stackCorruptionHook(kCurrentTask);
			}
		#else
			if (tasks_checkStackBounds(kCurrentTask) || memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
				kernel_stackCorruptionHook(kCurrentTask);
			}
		#endif
	#endif

	#if CFG_MEMORY_PROTECTION_MODE != 0
		if (memory_pointerSanityCheck((void*)kNextTask) != 0) {
			kernel_panic(PSTR("Memory access violation in task manager: kNextTask is out of bounds\r\n"));
		}
	#endif
	kCurrentTask = kNextTask;
}

kStackPtr_t tasks_getKernelStackPointer()
{
	return kStackPointer;
}

kStackPtr_t tasks_getReservedMemoryPointer()
{
	return kReservedMemory;
}

kTaskHandle_t tasks_getCurrentTaskHandle()
{
	return kCurrentTask;
}

kTaskHandle_t tasks_getNextTaskHandle()
{
	return kNextTask;
}

void tasks_setCurrentTask(kTaskHandle_t taskHandle)
{
	kCurrentTask = taskHandle;
}

void tasks_setNextTask(kTaskHandle_t taskHandle)
{
	kNextTask = taskHandle;
}

void tasks_setKernelStackPointer(kStackPtr_t pointer)
{
	kStackPointer = pointer;
}

void tasks_switchTask()
{
	tasks_schedule();
	if (kNextTask != kCurrentTask) tasks_switchContext();
}

void tasks_sleep(kTaskTicks_t sleep)
{
	tasks_setActiveTicks(0);

	if (sleep != 0) {
		tasks_setTaskState(kCurrentTask, KSTATE_SLEEPING);
		kCurrentTask -> sleepTime = sleep;
	}
	platform_yield();
}

void tasks_tick()
{
	tasks_switchTask();

	//kernel_timerService();

	__e_time++;
}