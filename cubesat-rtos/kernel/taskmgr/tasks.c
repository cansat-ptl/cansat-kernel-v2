/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */

#include <kernel/kernel.h>

static volatile uint8_t kGlobalPid = 1;
static volatile uint8_t kTaskIndex = 0;

static volatile kTaskHandle_t kTaskListHead;
static volatile kTaskHandle_t kTaskListTail;

kTaskHandle_t taskmgr_getTaskListPtr()
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	kTaskHandle_t temp = kTaskListHead;
	threads_endAtomicOperation(sreg);
	return temp;
}

uint8_t taskmgr_getTaskListIndex()
{
	return kTaskIndex;
}

void taskmgr_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state)
{
	if (t_handle != NULL) {
		t_handle -> state = t_state;
	}	
}

static inline void taskmgr_setupTaskStructure(kTaskHandle_t task, \
												kTask_t startupPointer, \
												kStackPtr_t stackPointer, \
												kStackSize_t stackSize, \
												void* args, \
												uint8_t priority, \
												kTaskState_t state, \
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
	task -> state = state;
	task -> type = type;
	task -> pid = kGlobalPid;
	task -> name = name;
}


void taskmgr_insertTask(kTaskHandle_t newTask)
{
	kTaskHandle_t current;

	if (kTaskListHead == NULL || kTaskListHead->priority <= newTask->priority) {
		newTask->next = kTaskListHead;
		kTaskListHead = newTask;
	}
	else {
		current = kTaskListHead;
		
		while (current->next != NULL && current->next->priority > newTask->priority) {
			current = current->next;
		}
		
		debug_logMessage(PGM_ON, L_INFO, PSTR("taskmgr: Inserting task to list, Cur = 0x%04X, Next = 0x%04X, New = 0x%04X\r\n"), current, current->next, newTask);
		
		newTask->next = current->next;
		current->next = newTask;
	}
}

void _debug_taskmgr_printTasks() 
{
	debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Current task list: "));
	kTaskHandle_t temp = kTaskListHead;
	while(temp != NULL)
	{
		debug_logMessage(PGM_ON, L_NONE, PSTR("name:%s,prio:%d,stack=0x%04X  "),temp->name, temp->priority, temp->stackPtr);
		temp = temp->next;
	}
	debug_logMessage(PGM_PUTS, L_NONE, PSTR("\r\n"));
}

uint8_t taskmgr_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (entry != NULL) {
		if (taskStruct != NULL) {
			if (stack != NULL) {
				taskmgr_setupTaskStructure(taskStruct, entry, stack, stackSize, args, priority, KSTATE_READY, type, name);
				platform_prepareStackFrame(stack, 0, entry, args);

				taskmgr_insertTask(taskStruct);
				
				kTaskIndex++;
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
	
	kTaskHandle_t taskStruct = (kTaskHandle_t)memmgr_heapAlloc(sizeof(struct kTaskStruct_t)); //TODO: 1 allocation
	kStackPtr_t stackPointer = (kStackPtr_t)memmgr_heapAlloc(stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN);
	if (stackPointer != NULL) stackPointer += stackSize + CFG_KERNEL_STACK_SAFETY_MARGIN + CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET;
				
	exitcode = taskmgr_createTaskStatic(taskStruct, stackPointer, entry, args, stackSize, priority, type, name);
	
	if (exitcode != 0) {
		memmgr_heapFree((void*)taskStruct);
		memmgr_heapFree((void*)stackPointer);
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

}
