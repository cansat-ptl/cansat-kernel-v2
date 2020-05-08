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

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
volatile uint64_t __e_time = 0;
extern volatile uint16_t _kflags;
extern volatile uint8_t _kTaskMgrFlags;
static volatile uint8_t kInterruptDepth = 0;
static volatile uint8_t kTickRate = 0;

static volatile uint8_t kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
static volatile kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-1];

volatile uint16_t kTaskActiveTicks = 0;

static inline void taskmgr_switchTask();
void taskmgr_schedule();

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

static inline void taskmgr_switchTask()
{	
	if (hal_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED))
		taskmgr_schedule();
	if (kNextTask != kCurrentTask) kernel_switchContext();
}

void taskmgr_yield(uint16_t sleep) 
{
	kernel_saveContext();
	
	if (sleep != 0) {
		kCurrentTask -> state = KSTATE_SLEEPING;
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
	
	if (kTickRate == 0) {
		if (kTaskActiveTicks != 0 && kCurrentTask -> priority != KPRIO_REALTIME  && kCurrentTask -> priority != KPRIO_IDLE)
			kTaskActiveTicks--;
		else
			taskmgr_switchTask();
		kTickRate = CFG_TICKRATE_MS;
	}
	else {
		kTickRate--;
	}
	
	kernel_timerService();
	
	__e_time++;
	
	hal_CLEAR_BIT(_kflags, KFLAG_TIMER_ISR);
	kernel_restoreContext();
	platform_RET();
}
