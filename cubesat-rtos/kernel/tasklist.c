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

kTaskHandle_t* kernel_getTaskListPtr()
{
	return kTaskQueue;
}

uint8_t kernel_getTaskListIndex()
{
	return kTaskIndex;
}

void kernel_resetTask(kTaskHandle_t taskList, uint8_t position)
{
	taskList[position].stackPtr = NULL;
	taskList[position].stackSize = 0;
	taskList[position].priority = 0;
	taskList[position].taskPtr = NULL;
	taskList[position].args = NULL;
	taskList[position].stackBegin = NULL;
	taskList[position].lock = NULL;
	taskList[position].state = KSTATE_UNINIT;
	taskList[position].type = KTASK_SYSTEM;
	taskList[position].pid = 0;
	taskList[position].name = NULL;
}

static inline void kernel_setupTaskStructure(kTaskHandle_t task, \
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

uint8_t kernel_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (taskStruct != NULL && stack != NULL && entry != NULL) {
		if (kTaskIndex < CFG_MAX_TASK_COUNT) {
			kernel_setupTaskStructure(taskStruct, entry, stack, stackSize, args, priority, type, name);
			platform_prepareStackFrame(stack, entry, args);

			kTaskQueue[kTaskIndex] = taskStruct;
			kTaskIndex++;

			exitcode = 0;
		}
		else {
			exitcode = 2;
		}
	}

	threads_endAtomicOperation(sreg);
	return exitcode;
}

uint8_t kernel_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (entry != NULL) {
		kTaskHandle_t taskStruct = (kTaskHandle_t)kernel_heapAlloc(sizeof(struct kTaskStruct_t));
		
		if (taskStruct  != NULL) {
			kStackPtr_t stackPointer = (kStackPtr_t)kernel_heapAlloc(stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN);
			if (stackPointer != NULL) {
				stackPointer += stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN + CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET;
				kernel_createTaskStatic(taskStruct, stackPointer, entry, args, stackSize, priority, type, name);
				*handle = taskStruct;
				exitcode = 0;
			}
			else {
				kernel_heapFree((void*)stackPointer);
				exitcode = 3;
			}
		}
		else {
			exitcode = 2;
		}
	}

	threads_endAtomicOperation(sreg);
	return exitcode;
}

kTaskHandle_t kernel_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	kTaskHandle_t returnValue = NULL;
	uint8_t ret = kernel_createTaskDynamic(&returnValue, entry, args, stackSize, priority, type, name);
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

uint8_t kernel_removeTask(kTaskHandle_t handle)
{

}
