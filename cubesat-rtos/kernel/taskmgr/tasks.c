/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */

#include <kernel/kernel.h>

static volatile kTaskHandle_t kTaskQueue[CFG_MAX_TASK_COUNT];
static volatile struct kTaskStruct_t kTaskList[CFG_MAX_TASK_COUNT];
static volatile uint8_t kGlobalPid = 1;
static volatile uint8_t kTaskIndex = 0;

kTaskHandle_t* taskmgr_getTaskListPtr()
{
	return kTaskQueue;
}

uint8_t taskmgr_getTaskListIndex()
{
	return kTaskIndex;
}

void taskmgr_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state)
{
	if (t_handle == NULL) {
		t_handle -> state = t_state;
	}	
}

void taskmgr_resetTask(kTaskHandle_t task)
{
	task -> stackPtr = NULL;
	task -> stackSize = 0;
	task -> priority = 0;
	task -> taskPtr = NULL;
	task -> args = NULL;
	task -> stackBegin = NULL;
	task -> lock = NULL;
	task -> state = KSTATE_UNINIT;
	task -> type = KTASK_SYSTEM;
	task -> pid = 0;
	task -> name = NULL;
}

static inline void taskmgr_setupTaskStructure(kTaskHandle_t task, \
											kTask_t startupPointer, \
											kStackPtr_t stackPointer, \
											kStackSize_t stackSize, \
											void* args, \
											uint8_t priority, \
											kTaskType_t type, \
											char* name)
{
	task -> stackPtr = stackPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET);
	task -> stackBegin = stackPointer;
	task -> stackSize = stackSize;
	task -> taskPtr = startupPointer;
	task -> args = args;
	task -> priority = priority;
	task -> lock = NULL;
	task -> state = KSTATE_READY;
	task -> type = type;
	task -> pid = kGlobalPid;
	task -> name = name;
}

uint8_t taskmgr_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (entry != NULL) {
		if (taskStruct != NULL) {
			if (stack != NULL) {
				if (kTaskIndex < CFG_MAX_TASK_COUNT) {
					stack += stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN;
					taskmgr_setupTaskStructure(taskStruct, entry, stack, stackSize, args, priority, type, name);
					platform_prepareStackFrame(stack + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET), entry, args);

					kTaskQueue[kTaskIndex] = taskStruct;
					kTaskIndex++;
					kGlobalPid++;

					exitcode = 0;
				}
				else {
					exitcode = 4;
				}
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
	
	kTaskHandle_t taskStruct = (kTaskHandle_t)kernel_heapAlloc(sizeof(struct kTaskStruct_t));
	kStackPtr_t stackPointer = (kStackPtr_t)kernel_heapAlloc(stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN);
				
	exitcode = taskmgr_createTaskStatic(taskStruct, stackPointer, entry, args, stackSize, priority, type, name);
	
	if (exitcode != 0) {
		kernel_heapFree((void*)taskStruct);
		kernel_heapFree((void*)stackPointer);
	}
	else {
		*handle = taskStruct;
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
}

kTaskHandle_t taskmgr_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	kTaskHandle_t returnValue = NULL;
	uint8_t ret = taskmgr_createTaskDynamic(&returnValue, entry, args, stackSize, priority, type, name);
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
	return returnValue;
}

uint8_t taskmgr_removeTask(kTaskHandle_t handle)
{

}
