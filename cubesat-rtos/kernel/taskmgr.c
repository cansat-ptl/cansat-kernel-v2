/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include <kernel.h>
#include <hal/hal.h>
#include "../kernel_config.h"
#include "avr/platform.h"

#define kernel_DISABLE_CONTEXT_SWITCH() hal_CLEAR_BIT(kflags, KFLAG_CSW_ALLOWED)
#define kernel_ENABLE_CONTEXT_SWITCH() hal_SET_BIT(kflags, KFLAG_CSW_ALLOWED)

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
static volatile uint16_t kflags = 0;
static volatile uint8_t taskMgrFlags = 0;
volatile uint64_t __e_time = 0;
static volatile uint8_t kInterruptDepth = 0;
static volatile uint8_t kGlobalPid = 1;

static volatile struct kTaskStruct_t kTaskList[MAX_TASK_COUNT];
static volatile uint8_t kTaskIndex = 0;

static volatile uint16_t kTaskActiveTicks = 0;
static volatile uint8_t kCurrentTaskIdx = 0; //TODO: task switch logic
static volatile uint8_t kPreviousTaskIdx = 0; //TODO: task switch logic

static inline void kernel_switchTask();
void kernel_switchTo(kTaskHandle_t handle) __attribute__ (( naked, noinline ));
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

uint8_t kernel_startAtomicOperation()
{
	kernel_enterCriticalSection();
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	return sreg;
}

void kernel_endAtomicOperation(uint8_t sreg)
{
	kernel_exitCriticalSection();
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
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
	kPreviousTaskIdx = kCurrentTaskIdx;
	
	if (hal_CHECK_BIT(kflags, KFLAG_CSW_ALLOWED)) {
		for (int i = 0; i < kTaskIndex && !switchReady; i++) {
			switch (kTaskList[i].state) {
				
				case KSTATE_READY:
					kNextTask = &kTaskList[i];
					switchReady = 1;
					if (kTaskList[i].priority == KPRIO_REALTIME)
						kTaskActiveTicks = 0;
					else
						kTaskActiveTicks = 1 + kTaskList[i].priority;
				break;
				
				case KSTATE_SLEEPING:
					if (hal_CHECK_BIT(kflags, KFLAG_TIMER_ISR)) {
						if (kTaskList[i].sleepTime != 0) {
							kTaskList[i].sleepTime--;
						}
						else {
							kTaskList[i].state = KSTATE_READY;
							i--;
						}
					}
				break;
				
				default:
				break;
			}
		}
	}
	if (kCurrentTask != kNextTask && switchReady) kernel_switchContext();
}

inline void kernel_stopTask(kTaskState_t exitState)
{
	kCurrentTask -> state = exitState;
	kernel_yield(0);
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
	kernel_RET();
}

void kernel_switchTo(kTaskHandle_t handle)
{
	kernel_saveContext();
	kernel_switchContext();
	kernel_restoreContext();
	kernel_RET();
}

static void kernel_tick()
{
	kernel_saveContext();
	hal_SET_BIT(kflags, KFLAG_TIMER_ISR);
	__e_time++;
	kernel_switchTask();
	//hal_startTimer0();
	hal_CLEAR_BIT(kflags, KFLAG_TIMER_ISR);
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
