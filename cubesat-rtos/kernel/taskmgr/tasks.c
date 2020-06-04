/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */

#include <kernel/kernel.h>

static volatile uint8_t kGlobalPid = 0;

static volatile kTaskHandle_t kTaskListHead;
static volatile kTaskHandle_t kTaskListTail;

static const size_t kTaskStructSize	= (sizeof(struct kTaskStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

static volatile struct kTaskStruct_t kIdleTaskStruct;

void taskmgr_setKernelStackPointer(kStackPtr_t pointer); //TODO: add to header
void taskmgr_setIdleTask(kTaskHandle_t idle);
void taskmgr_initScheduler(kTaskHandle_t idle);

kTaskHandle_t taskmgr_getTaskListPtr()
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	kTaskHandle_t temp = kTaskListHead;
	threads_endAtomicOperation(sreg);
	return temp;
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
	
	kTaskListHead = &kIdleTaskStruct;
	kTaskListTail = &kIdleTaskStruct;
	
	taskmgr_initScheduler(&kIdleTaskStruct);
	taskmgr_setCurrentTask(&kIdleTaskStruct);
	taskmgr_setNextTask(&kIdleTaskStruct);
	
	return 0;
}

void taskmgr_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	if (task != NULL) {
		task->state = state;
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

static void taskmgr_addTaskToTaskList(kTaskHandle_t newTask) 
{

}

static void taskmgr_removeTaskFromTaskList(kTaskHandle_t task) 
{
	
}

void _debug_taskmgr_printTasks() 
{
	debug_logMessage(PGM_PUTS, L_INFO, PSTR("taskmgr: Current task list: \r\n"));
	kTaskHandle_t temp = kTaskListHead;
	while(temp != NULL)
	{
		debug_logMessage(PGM_ON, L_NONE, PSTR("name:%s,prio:%d,stack=0x%04X  \r\n"),temp->name, temp->priority, temp->stackPtr);
		temp = temp->taskList.next;
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
				kStackPtr_t stackPrepared = platform_prepareStackFrame(stack, stackSize, entry, args);
				taskmgr_setupTaskStructure(taskStruct, entry, stackPrepared, stack, stackSize, args, priority, KSTATE_READY, type, name);

				taskmgr_addTaskToTaskList(taskStruct);

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
		taskmgr_removeTaskFromTaskList(handle);
		taskmgr_unscheduleTask(handle);
		
		memmgr_heapFree((void*)handle);
	}
	
	threads_endAtomicOperation(sreg);
}
