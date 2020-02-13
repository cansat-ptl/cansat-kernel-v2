/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include <kernel.h>
#include "../kernel_config.h"
#include "avr/platform.h"

#define kernel_DISABLE_CONTEXT_SWITCH() hal_CLEAR_BIT(kflags, KFLAG_CSW_ALLOWED)
#define kernel_ENABLE_CONTEXT_SWITCH() hal_SET_BIT(kflags, KFLAG_CSW_ALLOWED)

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
static volatile uint16_t kflags = 0;
volatile uint64_t __e_time = 0;
static volatile uint8_t kInterruptDepth = 0;
static volatile uint8_t kGlobalPid = 1;

static volatile struct kTaskStruct_t kTaskList[MAX_TASK_COUNT];
static volatile uint8_t kTaskIndex = 0;

static volatile uint8_t kCurrentTaskIdx = 0; //TODO: task switch logic

static inline void kernel_switchTask();
static void kernel_tick()  __attribute__ ( ( naked, noinline ));

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

void kernel_enterCriticalSection()
{
	kernel_DISABLE_CONTEXT_SWITCH();
}

void kernel_exitCriticalSection()
{
	kernel_ENABLE_CONTEXT_SWITCH();
}

inline static void kernel_ISREnter()
{
	kInterruptDepth++;
}

inline static void kernel_ISRExit()
{
	if(kInterruptDepth) kInterruptDepth--;
}

void kernel_setFlag(uint8_t flag, uint8_t value)
{
	hal_DISABLE_INTERRUPTS();
	hal_WRITE_BIT(kflags, flag, value);
	hal_ENABLE_INTERRUPTS();
}

uint8_t kernel_checkFlag(uint8_t flag)
{
	hal_DISABLE_INTERRUPTS();
	uint8_t res = hal_CHECK_BIT(kflags, flag);
	hal_ENABLE_INTERRUPTS();
	return res;
}

uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskStatus_t t_state)
{
	if (t_handle == NULL) return ERR_NULLPTR;
	
	t_handle -> status = t_state;
	
	return 0;
}

/*typedef struct taskStruct {
void *stackPtr;
void *statusPtr;
task taskPtr;
taskPriority_t priority;
taskStatus_t status;
taskType_t type;
uint8_t pid;
const char name[8];
};*/

kTaskHandle_t kernel_createTask(kTask_t startupPointer, kStackSize_t taskStackSize, kTaskPriority_t taskPriority, kTaskType_t taskType)
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
	kTaskList[kTaskIndex].status = KSTATE_READY;
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
	if (hal_CHECK_BIT(kflags, KFLAG_CSW_ALLOWED)) {
		kCurrentTask = kNextTask;
	}
}

static inline void kernel_switchTask()
{	
	if (kCurrentTaskIdx == kTaskIndex-1)
	kCurrentTaskIdx = 0;
	
	kNextTask = &kTaskList[kCurrentTaskIdx];
	kCurrentTaskIdx++;
	
	if (kNextTask != kCurrentTask && kNextTask -> stackPtr != NULL && kNextTask -> taskPtr != NULL) kernel_switchContext();
}

void kernel_yield() 
{
	kernel_saveContext();
	kernel_switchTask();
	kernel_restoreContext();
	kernel_RET();
}

static void kernel_tick()
{
	kernel_saveContext();
	__e_time++;
	kernel_switchTask();
	//hal_startTimer0();
	kernel_restoreContext();
	kernel_RET();
}

ISR(TIMER0_COMP_vect, ISR_NAKED)
{
	//kernel_taskService();
	kernel_tick();
	//kernel_taskSwitch();
	kernel_RETI();
}
