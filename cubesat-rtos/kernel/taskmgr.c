/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include <platform/platform.h>
#include <kernel.h>
#include <hal/hal.h>
#include "../kernel_config.h"

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
volatile uint64_t __e_time = 0;
extern volatile uint16_t _kflags;
extern volatile uint8_t _kTaskMgrFlags;
static volatile uint8_t kInterruptDepth = 0;
volatile kTaskHandle_t kIdleTaskHandle;

static volatile uint16_t kTaskActiveTicks = 0;

static inline void kernel_switchTask();
void kernel_schedule();
void kernel_switchTo(kTaskHandle_t handle) __attribute__ (( naked, noinline ));

kTaskHandle_t kernel_getCurrentTaskHandle()
{
	return kCurrentTask;
}

kTaskHandle_t kernel_getNextTaskHandle()
{
	return kNextTask;
}

void kernel_setCurrentTask(kTaskHandle_t taskHandle) 
{
	kCurrentTask = taskHandle;
}

void kernel_setNextTask(kTaskHandle_t taskHandle)
{
	kNextTask = taskHandle;
}


inline static void kernel_ISREnter()
{
	kInterruptDepth++;
}

inline static void kernel_ISRExit()
{
	if(kInterruptDepth) kInterruptDepth--;
}

uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state)
{
	if (t_handle == NULL) return ERR_NULLPTR;
	
	t_handle -> state = t_state;
	
	return 0;
}

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
		kCurrentTask = kNextTask;
}

static inline void kernel_switchTask()
{	
	if (hal_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED))
		kernel_schedule();
	if (kNextTask != kCurrentTask) kernel_switchContext();
}

void kernel_yield(uint16_t sleep) 
{
	kernel_saveContext();
	
	if (sleep != 0) {
		kCurrentTask -> state = KSTATE_SLEEPING;
		kCurrentTask -> sleepTime = sleep;
	}
	
	kernel_switchTask();
	kernel_restoreContext();
	platform_RET();
}

void kernel_tick()
{
	kernel_saveContext();
	
	hal_SET_BIT(_kflags, KFLAG_TIMER_ISR);
	
	if (kTaskActiveTicks != 0 && kCurrentTask -> priority != KPRIO_REALTIME)
		kTaskActiveTicks--;
	else {
		kernel_switchTask();
	}
	
	__e_time++;
	
	hal_CLEAR_BIT(_kflags, KFLAG_TIMER_ISR);
	kernel_restoreContext();
	platform_RET();
}
