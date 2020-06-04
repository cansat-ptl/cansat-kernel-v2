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
#include "listutils.h"

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
volatile uint64_t __e_time = 0;
extern volatile uint16_t _kflags;
static volatile uint8_t kInterruptDepth = 0;

static volatile uint8_t kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
static volatile kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-1];

static inline void taskmgr_switchTask();
void taskmgr_schedule();
void taskmgr_setActiveTicks(uint16_t activeTicks);
void taskmgr_setKernelTicks(uint16_t activeTicks);

inline void kernel_saveContext()
{
	platform_SAVE_CONTEXT();
}

inline void kernel_restoreContext()
{
	platform_RESTORE_CONTEXT();
}

static inline void kernel_switchContext()
{
	#if CFG_ENABLE_MEMORY_PROTETCTION == 1
		kernel_checkStackProtectionRegion(taskmgr_getCurrentTaskHandle());
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

static inline void taskmgr_switchTask()
{	
	taskmgr_schedule();
	if (kNextTask != kCurrentTask) kernel_switchContext();
}

void taskmgr_yield(uint16_t sleep) 
{
	kernel_saveContext();
	
	taskmgr_setActiveTicks(0);
	
	if (sleep != 0) {
		taskmgr_setTaskState(kCurrentTask, KSTATE_SLEEPING);
		kCurrentTask -> sleepTime = sleep;
	}
	
	taskmgr_switchTask();
	kernel_restoreContext();
	platform_RET();
}

void taskmgr_switchTo(kTaskHandle_t task)
{
	kernel_saveContext();
	
	if (task == NULL) {
		kernel_restoreContext();
		platform_RET();
	}
		
	kNextTask = task;
	if (kNextTask != kCurrentTask) kernel_switchContext();
	
	kernel_restoreContext();
	platform_RET();
}

void taskmgr_tick()
{
	kernel_saveContext();
	
	hal_SET_BIT(_kflags, KFLAG_TIMER_ISR);
	
	taskmgr_switchTask();
	
	kernel_timerService();
	
	__e_time++;
	
	hal_CLEAR_BIT(_kflags, KFLAG_TIMER_ISR);
	kernel_restoreContext();
	platform_RET();
}
