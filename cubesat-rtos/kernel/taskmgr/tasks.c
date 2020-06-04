/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */

#include <kernel/kernel.h>
#include "listutils.h"

static volatile uint16_t kGlobalPid = 0;

static volatile struct kLinkedListStruct_t kReadyTaskList[CFG_NUMBER_OF_PRIORITIES];
static volatile struct kLinkedListStruct_t kSleepingTaskList;
static volatile struct kLinkedListStruct_t kSuspendedTaskList;

static const size_t kTaskStructSize	= (sizeof(struct kTaskStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

static volatile struct kTaskStruct_t kIdleTaskStruct;

void taskmgr_setKernelStackPointer(kStackPtr_t pointer); //TODO: add to header
void taskmgr_setIdleTask(kTaskHandle_t idle);
void taskmgr_initScheduler(kTaskHandle_t idle);

volatile struct kLinkedListStruct_t* taskmgr_getReadyTaskListArray()
{
	return kReadyTaskList;
}

volatile struct kLinkedListStruct_t* taskmgr_getReadyTaskListPtr(uint8_t priority)
{
	return &kReadyTaskList[priority];
}

volatile struct kLinkedListStruct_t* taskmgr_getSleepingTaskListPtr()
{
	return &kSleepingTaskList;
}

kTaskHandle_t taskmgr_getIdleTaskHandle()
{
	return &kIdleTaskStruct;
}

uint8_t taskmgr_init(kTask_t idle)
{
	kStackPtr_t rMemory = taskmgr_getReservedMemoryPointer();
	
	uint8_t result = taskmgr_createTaskStatic(&kIdleTaskStruct, rMemory, idle, NULL, CFG_KERNEL_RESERVED_MEMORY, KPRIO_IDLE, KTASK_SYSTEM, "idle");
	
	if (result != 0) {
		debug_logMessage(PGM_PUTS, L_FATAL, PSTR("\r\ntaskmgr: Startup failed, could not create idle task.\r\n"));
		while(1);
	}
	
	rMemory += CFG_KERNEL_RESERVED_MEMORY + CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET;
	taskmgr_setKernelStackPointer(rMemory);
		
	taskmgr_initScheduler(&kIdleTaskStruct);
	taskmgr_setCurrentTask(&kIdleTaskStruct);
	taskmgr_setNextTask(&kIdleTaskStruct);
	
	return 0;
}

void taskmgr_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	if (task != NULL) {
		switch (state) {
			case KSTATE_UNINIT:
				taskmgr_listDeleteAny(task->taskList.list, task);
				task->state = KSTATE_UNINIT;
			break;
			case KSTATE_SUSPENDED:
				taskmgr_listDeleteAny(task->taskList.list, task);
				taskmgr_listAddBack(&kSuspendedTaskList, task);
				task->state = KSTATE_SUSPENDED;
			break;
			case KSTATE_SLEEPING:
				taskmgr_listDeleteAny(task->taskList.list, task);
				taskmgr_listAddBack(&kSleepingTaskList, task);
				task->state = KSTATE_SLEEPING;
			break;
			case KSTATE_BLOCKED:
				//Do nothing, blocking is handled by threads module
				//This should also be logged
			break;
			case KSTATE_READY:
				taskmgr_listDeleteAny(task->taskList.list, task);
				taskmgr_listAddBack(&kReadyTaskList[task->priority], task);
				task->state = KSTATE_READY;
			break;
			case KSTATE_RUNNING:
				task->state = KSTATE_RUNNING;
			break;
			default:
				//Unknown state, should be logged
				//TODO: log state error
			break;
		}
	}
	
	threads_endAtomicOperation(sreg);
}

uint8_t taskmgr_setTaskPriority(kTaskHandle_t task, uint8_t priority)
{
	uint8_t exitcode = 0;
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	if (task != NULL) {
		if (priority <= CFG_NUMBER_OF_PRIORITIES) {
			task->priority = priority;
			if (task->state == KSTATE_READY) {
				taskmgr_setTaskState(task, KSTATE_READY);
			}
		}
		else {
			exitcode = CFG_NUMBER_OF_PRIORITIES;
		}
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
}

static inline void taskmgr_setupTaskStructure(kTaskHandle_t task, \
												kTask_t startupPointer, \
												kStackPtr_t stackPointer, \
												kStackPtr_t stackBegin, \
												kStackSize_t stackSize, \
												void* args, \
												uint8_t priority, \
												kTaskState_t state, \
												kTaskType_t type, \
												char* name)
{
	task -> stackPtr = stackPointer;
	task -> stackBegin = stackBegin;
	task -> stackSize = stackSize;
	task -> taskPtr = startupPointer;
	task -> args = args;
	task -> priority = priority;
	task -> lock = NULL;
	task -> state = state;
	task -> type = type;
	task -> pid = kGlobalPid;
	task -> name = name;
	task -> taskList.next = NULL;
	task -> taskList.prev = NULL;
}

uint8_t taskmgr_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (entry != NULL) {
		if (taskStruct != NULL) {
			if (stack != NULL) {
				kStackPtr_t stackPrepared = platform_prepareStackFrame(stack, stackSize, entry, args);
				taskmgr_setupTaskStructure(taskStruct, entry, stackPrepared, stack, stackSize, args, priority, KSTATE_READY, type, name);

				taskmgr_setTaskState(taskStruct, KSTATE_READY);

				kGlobalPid++;
				exitcode = 0;
			}
			else {
				exitcode = 2;
			}
		}
		else {
			exitcode = 3;
		}	
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
}

uint8_t taskmgr_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	kStackPtr_t stackPointer = (kStackPtr_t)memmgr_heapAlloc(stackSize + kTaskStructSize);
				
	exitcode = taskmgr_createTaskStatic((kTaskHandle_t)stackPointer, stackPointer + kTaskStructSize, entry, args, stackSize, priority, type, name);
	
	if (exitcode != 0) {
		memmgr_heapFree((void*)stackPointer);
	}
	else {
		*handle = (kTaskHandle_t)stackPointer;
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
}

kTaskHandle_t taskmgr_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	kTaskHandle_t returnValue = NULL;
	uint8_t ret = taskmgr_createTaskDynamic(&returnValue, entry, args, stackSize, priority, type, name);
	#if CFG_LOGGING == 1
	switch (ret) {
		case 0:
			debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Successfully created a new task\r\n"));
			break;
		case 1:
			debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Task creation error[1]: entryPoint is NULL\r\n"));
			break;
		case 2:
			debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Task creation error[2]: failed to allocate task heap\r\n"));
			break;
		case 3:
			debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Task creation error[3]: failed to allocate task structure\r\n"));
			break;
		default:
			debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Task creation error[]: unknown error\r\n"));
			break;
	}
	#endif
	return returnValue;
}

uint8_t taskmgr_removeTask(kTaskHandle_t handle)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	if (handle != NULL) {
		taskmgr_setTaskState(handle, KSTATE_UNINIT);
		memmgr_heapFree((void*)handle);
	}
	
	threads_endAtomicOperation(sreg);
	return 0;
}
