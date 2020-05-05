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
	taskList[position].args = NULL;
	taskList[position].stackBegin = NULL;
	taskList[position].lock = NULL;
	taskList[position].state = KSTATE_UNINIT;
	taskList[position].type = KTASK_SYSTEM;
	taskList[position].pid = 0;
	taskList[position].name = NULL;
}

static inline void kernel_setupTaskStructure(kTaskHandle_t taskList, \
											uint8_t taskIndex, \
											kTask_t startupPointer, \
											kStackPtr_t stackPointer, \
											kStackSize_t stackSize, \
											void* args, \
											uint8_t priority, \
											kTaskType_t type, \
											char* name)
{
	taskList[taskIndex].stackPtr = stackPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET);
	taskList[taskIndex].stackBegin = stackPointer;
	taskList[taskIndex].stackSize = stackSize;
	taskList[taskIndex].taskPtr = startupPointer;
	taskList[taskIndex].args = args;
	taskList[taskIndex].priority = priority;
	taskList[taskIndex].lock = NULL;
	taskList[taskIndex].state = KSTATE_READY;
	taskList[taskIndex].type = type;
	taskList[taskIndex].pid = kGlobalPid;
	taskList[taskIndex].name = name;
}

kTaskHandle_t kernel_createTask(kTask_t startupPointer, void* args, kStackSize_t taskStackSize, uint8_t taskPriority, kTaskType_t taskType, char* name)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	kTaskHandle_t handle = &kTaskList[kTaskIndex];

	#if CFG_LOGGING == 1
		debug_logMessage(PGM_ON, L_INFO, PSTR("taskmgr: Registering new task, PID=%d, StartPTR=0x%08X, Stack=%d, Prio=%d\r\n"), kGlobalPid, startupPointer, taskStackSize, taskPriority);
	#endif

	if (startupPointer != NULL) {
		#if CFG_LOGGING == 1
			debug_puts(L_INFO, PSTR("kernel: Memory allocation"));
		#endif

		kStackPtr_t stackPointer = kernel_setupTaskStack(startupPointer, taskStackSize, taskType, args);

		if (stackPointer != NULL) {
			#if CFG_LOGGING == 1
				debug_puts(L_NONE, PSTR("                             [OK]\r\n"));
			#endif

			kernel_setupTaskStructure(kTaskList, kTaskIndex, startupPointer, stackPointer, taskStackSize, args, taskPriority, taskType, name);

			kGlobalPid++;
			kTaskIndex++;

			#if CFG_LOGGING == 1
				debug_puts(L_INFO, PSTR("taskmgr: Task registration                            [OK]\r\n"));
			#endif
		}
		else {
			#if CFG_LOGGING == 1
				debug_puts(L_NONE, PSTR("                             [ERR]\r\n"));
			#endif
		}
	}
	
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
		kTaskIndex--;

		exitcode = 0;
	}

	threads_endAtomicOperation(sreg);
	return exitcode;
}
