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
static volatile uint8_t kGlobalPid = 1;
volatile kTaskHandle_t kIdleTaskHandle;

static volatile struct kTaskStruct_t kTaskList[MAX_TASK_COUNT];
static volatile uint8_t kTaskIndex = 0;

static volatile uint16_t kTaskActiveTicks = 0;
static volatile uint8_t kCurrentTaskIdx = 0; //TODO: task switch logic
static volatile uint8_t kPreviousTaskIdx = 0; //TODO: task switch logic

static inline void kernel_switchTask();
void kernel_switchTo(kTaskHandle_t handle) __attribute__ (( naked, noinline ));

kTaskHandle_t kernel_getCurrentTaskHandle()
{
	return kCurrentTask;
}

kTaskHandle_t kernel_getNextTaskHandle()
{
	return kNextTask;
}

kTaskHandle_t kernel_getTaskListPtr()
{
	return kTaskList;
}

uint8_t kernel_getTaskListIndex()
{
	return kTaskIndex;
}

uint8_t kernel_getTaskPosition(kTaskHandle_t taskHandle)
{
	return taskHandle -> position;
}

void kernel_setCurrentTask(kTaskHandle_t taskHandle) 
{
	kCurrentTask = taskHandle;
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

kTaskHandle_t kernel_createTask(kTask_t startupPointer, kStackSize_t taskStackSize, uint8_t taskPriority, kTaskType_t taskType)
{	
	if (startupPointer == NULL) return NULL;

	kStackPtr_t stackPointer = kernel_setupTaskStack(startupPointer, taskStackSize, taskType);
	
	if (stackPointer == NULL) return NULL;
	
	kTaskList[kTaskIndex].stackPtr = stackPointer + (KERNEL_STACK_FRAME_REGISTER_OFFSET-31);
	kTaskList[kTaskIndex].stackSize = taskStackSize;
	kTaskList[kTaskIndex].priority = taskPriority;
	kTaskList[kTaskIndex].taskPtr = startupPointer;
	kTaskList[kTaskIndex].stackBegin = stackPointer;
	kTaskList[kTaskIndex].lock = NULL;
	kTaskList[kTaskIndex].state = KSTATE_READY;
	kTaskList[kTaskIndex].type = taskType;
	kTaskList[kTaskIndex].pid = kGlobalPid;
	
	kernel_sortTaskList(kTaskList, kTaskIndex); //Bruh
	
	kTaskHandle_t handle = NULL;
	
	for (int i = 0; i < kTaskIndex+1; i++) {
		if (kTaskList[i].pid == kGlobalPid) {
			handle = &kTaskList[i];
			break;
		}
	}
	
	kGlobalPid++;
	kTaskIndex++;

	return handle;
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
	uint8_t switchReady = 0;
	/*
	if (hal_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED)) {
		for (int i = 0; i < kTaskIndex-1; i++) {
			if (kTaskList[i].state == KSTATE_READY) {
				if (kTaskList[i].priority >= kCurrentTask -> priority || kCurrentTask -> state != KSTATE_READY) {
					if (kCurrentTaskIdx < i)
				}
			}
		}

		
	}*/
	kernel_switchContext();
}

void kernel_yield(uint16_t sleep) 
{
	kernel_saveContext();
	
	if (sleep != 0) {
		kCurrentTask -> state = KSTATE_SLEEPING;
		kCurrentTask -> sleepTime = sleep;
	}
	else if (kCurrentTask -> state == KSTATE_READY) {
		kCurrentTask -> state = KSTATE_SUSPENDED;
		kCurrentTask -> sleepTime = 0;
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
