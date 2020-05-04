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

void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount) //Heap sort
{
	heapSort(taskList, amount); 
}

void heapify(kTaskHandle_t arr[], int n, int i) 
{ 
	int smallest = i; // Initialize smallest as root 
	int l = 2*i + 1; // left = 2*i + 1 
	int r = 2*i + 2; // right = 2*i + 2 
	if (l < n && arr[l].priority < arr[smallest].priority) // If left child is larger than root 
		smallest = l; 
	if (r < n && arr[r].priority < arr[smallest].priority) // If right child is larger than smallest so far 
		smallest = r; 
	if (smallest != i) // If smallest is not root 
	{ 
		kernel_swapTasks(&arr[i], &arr[smallest]); 
		heapify(arr, n, smallest); // Recursively heapify the affected sub-tree 
	} 
} 


void heapSort(kTaskHandle_t arr[], int n) 
{ 
	for (int i = n / 2 - 1; i >= 0; i--) 
		heapify(arr, n, i); 
	for (int i=n-1; i>0; i--) 
	{ 
		kernel_swapTasks(&arr[0], &arr[i]);
		heapify(arr, i, 0); 
	} 
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
