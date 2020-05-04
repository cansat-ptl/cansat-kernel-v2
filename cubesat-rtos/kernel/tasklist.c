/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */ 

#include <kernel/kernel.h>

static volatile struct kTaskStruct_t kTaskList[CFG_MAX_TASK_COUNT];
static volatile uint8_t kGlobalPid = 1;
static volatile uint8_t kTaskIndex = 0;

kTaskHandle_t kernel_getTaskListPtr()
{
	return kTaskList;
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
	taskList[position].stackBegin = NULL;
	taskList[position].lock = NULL;
	taskList[position].state = KSTATE_UNINIT;
	taskList[position].type = KTASK_SYSTEM;
	taskList[position].pid = 0;
}

inline void kernel_swapTasks(volatile struct kTaskStruct_t * taskA, volatile struct kTaskStruct_t * taskB)
{
	struct kTaskStruct_t taskTemp = *taskA;
	*taskA = *taskB;
	*taskB = taskTemp;
}

void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount) //Bubble sort
{
	uint8_t swapFlag = 0;
	
	for (int i = 0; i < amount; i++) {
		swapFlag = 0;
		for (int j = 0; j < amount-i; j++) {
			if (taskList[j].priority < taskList[j+1].priority) {
				swapFlag = 1;
				kernel_swapTasks(&taskList[j], &taskList[j+1]); //Bruh
			}
		}
		if (!swapFlag) break;
	}
	
	return;
}

kTaskHandle_t kernel_createTask(kTask_t startupPointer, void* args, kStackSize_t taskStackSize, uint8_t taskPriority, kTaskType_t taskType, char* name)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	#if CFG_LOGGING == 1
		debug_logMessage(PGM_ON, L_INFO, PSTR("taskmgr: Registering new task, PID=%d, StartPTR=0x%08X, Stack=%d, Prio=%d\r\n"), kGlobalPid, startupPointer, taskStackSize, taskPriority);
	#endif
	
	if (startupPointer == NULL) {
		#if CFG_LOGGING == 1
			debug_puts(L_INFO, PSTR("taskmgr: Task registration error: PTR=NULL\r\n"));
		#endif
		threads_endAtomicOperation(sreg);
		return NULL;
	}
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("kernel: Memory allocation"));
	#endif
	
	kStackPtr_t stackPointer = kernel_setupTaskStack(startupPointer, taskStackSize, taskType, args);
	
	if (stackPointer == NULL) {
		#if CFG_LOGGING == 1
			debug_puts(L_NONE, PSTR("                             [ERR]\r\n"));
		#endif
		threads_endAtomicOperation(sreg);
		return NULL;
	}
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                             [OK]\r\n"));
	#endif
	
	kTaskList[kTaskIndex].stackPtr = stackPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET);
	kTaskList[kTaskIndex].stackBegin = stackPointer;
	kTaskList[kTaskIndex].stackSize = taskStackSize;
	kTaskList[kTaskIndex].taskPtr = startupPointer;
	kTaskList[kTaskIndex].args = args;
	kTaskList[kTaskIndex].priority = taskPriority;
	kTaskList[kTaskIndex].lock = NULL;
	kTaskList[kTaskIndex].state = KSTATE_READY;
	kTaskList[kTaskIndex].type = taskType;
	kTaskList[kTaskIndex].pid = kGlobalPid;
	strcpy((char*)kTaskList[kTaskIndex].name, name);
	
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
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("taskmgr: Task registration                            [OK]\r\n"));
	#endif
	
	threads_endAtomicOperation(sreg);
	return handle;
}

uint8_t kernel_removeTask(kTaskHandle_t handle)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();
	kTaskHandle_t taskList = kernel_getTaskListPtr();
	uint8_t idx = utils_ARRAY_INDEX_FROM_ADDR(taskList, handle, struct kTaskStruct_t);
	
	if (handle != NULL) {
		kernel_resetTask(taskList, idx);
		kernel_sortTaskList(taskList, kTaskIndex);
		kTaskIndex--;
		
		exitcode = 0;
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
}