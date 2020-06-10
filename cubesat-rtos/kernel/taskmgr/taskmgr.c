/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include <kernel/platform/platform.h>
#include <kernel/kernel.h>
#include <kernel/hal/hal.h>
#include "../kernel_config.h"

kTaskHandle_t kCurrentTask;
static kTaskHandle_t kNextTask;
volatile uint64_t __e_time = 0;
extern volatile uint16_t _kflags;
static volatile uint8_t kInterruptDepth = 0;

extern volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
extern kStackPtr_t kStackPointer;

void taskmgr_schedule();
void taskmgr_setActiveTicks(uint16_t activeTicks);

void kernel_stackCorruptionHook(kTaskHandle_t task);

static void taskmgr_switchContext()
{
	#if CFG_MEMORY_PROTECTION_MODE == 1
		if (platform_checkStackBounds(kCurrentTask)) {
			kernel_stackCorruptionHook(kCurrentTask);
		}
	#endif
	
	#if CFG_MEMORY_PROTECTION_MODE == 2
		if (memmgr_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
			kernel_stackCorruptionHook(kCurrentTask);
		}
	#endif
	
	#if CFG_MEMORY_PROTECTION_MODE == 3
		if (platform_checkStackBounds(kCurrentTask) || memmgr_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
			kernel_stackCorruptionHook(kCurrentTask);
		}
	#endif
	
	#if CFG_MEMORY_PROTECTION_MODE != 0
		if (memmgr_pointerSanityCheck((void*)kNextTask) != 0) {
			kernel_panic(PSTR("Memory access violation in task manager: kNextTask is out of bounds\r\n"));
		}
	#endif
	kCurrentTask = kNextTask;
}

kStackPtr_t taskmgr_getKernelStackPointer()
{
	return kStackPointer;
}

kStackPtr_t taskmgr_getReservedMemoryPointer()
{
	return kReservedMemory;
}

kTaskHandle_t taskmgr_getCurrentTaskHandle()
{
	return kCurrentTask;
}

kTaskHandle_t taskmgr_getNextTaskHandle()
{
	return kNextTask;
}

void taskmgr_setCurrentTask(kTaskHandle_t taskHandle) 
{
	kCurrentTask = taskHandle;
}

void taskmgr_setNextTask(kTaskHandle_t taskHandle)
{
	kNextTask = taskHandle;
}

void taskmgr_setKernelStackPointer(kStackPtr_t pointer)
{
	kStackPointer = pointer;
}

void taskmgr_switchTask()
{	
	taskmgr_schedule();
	if (kNextTask != kCurrentTask) taskmgr_switchContext();
}

void taskmgr_sleep(kTaskTicks_t sleep)
{
	taskmgr_setActiveTicks(0);
	
	if (sleep != 0) {
		taskmgr_setTaskState(kCurrentTask, KSTATE_SLEEPING);
		kCurrentTask -> sleepTime = sleep;
	}
	platform_yield();
}

void taskmgr_tick()
{
	taskmgr_switchTask();
	
	kernel_timerService();
	
	__e_time++;
}