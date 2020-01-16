/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../kernel_config.h"

#define kernel_ret() asm volatile ("ret \n\t" ::)
#define kernel_reti() asm volatile ("reti \n\t" ::)
#define kernel_disableContextSwitch_m() hal_clearBit_m(kflags, KFLAG_CSW_ALLOWED)
#define kernel_enableContextSwitch_m() hal_setBit_m(kflags, KFLAG_CSW_ALLOWED)


volatile struct kTaskStruct_t *kCurrentTask;
volatile struct kTaskStruct_t *kNextTask;
static volatile uint16_t kflags = 0;
static volatile uint64_t e_time = 0;
static volatile uint8_t kInterruptDepth = 0;
static volatile uint8_t kGlobalPid = 1;
extern uint8_t mcucsr_mirror;

static volatile struct kTaskStruct_t kTaskList[MAX_TASK_COUNT];
static volatile uint8_t kTaskIndex = 0;

static volatile uint8_t kernelStack[KERNEL_STACK_SIZE];
static volatile uint16_t kStackUsage = 0;

void kernel_idle() 
{
	while(1) hal_nop();
}

void kernel_setFlag(uint8_t flag, uint8_t value)
{
	kernel_disableContextSwitch_m();
	hal_writeBit_m(kflags, flag, value);
	kernel_enableContextSwitch_m();
} 

uint8_t kernel_checkFlag(uint8_t flag)
{
	kernel_disableContextSwitch_m();
	uint8_t res = hal_checkBit_m(kflags, flag);
	kernel_enableContextSwitch_m();
	return res;	
}

uint64_t kernel_getUptime()
{
	return e_time;
}

uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskStatus_t t_state)
{
	if(t_handle == NULL) return ERR_NULLPTR;
	
	t_handle -> status = t_state;
	
	return 0;
}

static inline void kernel_saveContext()
{
	asm volatile (
		";--------Context-Save-------\n\t"\
		"push r0					\n\t" \
		"in r0, %[_SREG_]			\n\t" \
		"cli						\n\t" \
		"push r0					\n\t" \
		"push r1					\n\t" \
		"clr r1						\n\t" \
		"push r2					\n\t" \
		"push r3					\n\t" \
		"push r4					\n\t" \
		"push r5					\n\t" \
		"push r6					\n\t" \
		"push r7					\n\t" \
		"push r8					\n\t" \
		"push r9					\n\t" \
		"push r10					\n\t" \
		"push r11					\n\t" \
		"push r12					\n\t" \
		"push r13					\n\t" \
		"push r14					\n\t" \
		"push r15					\n\t" \
		"push r16					\n\t" \
		"push r17					\n\t" \
		"push r18					\n\t" \
		"push r19					\n\t" \
		"push r20					\n\t" \
		"push r21					\n\t" \
		"push r22					\n\t" \
		"push r23					\n\t" \
		"push r24					\n\t" \
		"push r25					\n\t" \
		"push r26					\n\t" \
		"push r27					\n\t" \
		"push r28					\n\t" \
		"push r29					\n\t" \
		"push r30					\n\t" \
		"push r31					\n\t" \
		"lds r26, kCurrentTask		\n\t" \
		"lds r27, kCurrentTask+1	\n\t" \
		"in r0, %[_SPL_]			\n\t" \
		"st X+, r0					\n\t" \
		"in r0, %[_SPH_]			\n\t" \
		"st X+, r0					\n\t" \
		";---------------------------\n\t"
		:
		:	[_SREG_] "i" _SFR_IO_ADDR(hal_statusReg),
			[_SPL_] "i" _SFR_IO_ADDR(SPL),
			[_SPH_] "i" _SFR_IO_ADDR(SPH)
	);
}

static inline void kernel_switchContext()
{
	asm volatile (
		"lds  r26, kNextTask \n\t"	\
		"lds  r27, kNextTask+1 \n\t" \
		"sts  kCurrentTask, r26 \n\t" \
		"sts  kCurrentTask+1, r27 \n\t"
		:
		:
	);
}

static inline void kernel_restoreContext()
{
	asm volatile (
		";------Context-Restore------\n\t"\
		"lds r26, kCurrentTask		\n\t" \
		"lds r27, kCurrentTask+1	\n\t" \
		"ld r0, X+					\n\t" \
		"out %[_SPL_],r0			\n\t" \
		"ld r0, X+					\n\t" \
		"out %[_SPH_],r0			\n\t" \
		"pop r31					\n\t" \
		"pop r30					\n\t" \
		"pop r29					\n\t" \
		"pop r28					\n\t" \
		"pop r27					\n\t" \
		"pop r26					\n\t" \
		"pop r25					\n\t" \
		"pop r24					\n\t" \
		"pop r23					\n\t" \
		"pop r22					\n\t" \
		"pop r21					\n\t" \
		"pop r20					\n\t" \
		"pop r19					\n\t" \
		"pop r18					\n\t" \
		"pop r17					\n\t" \
		"pop r16					\n\t" \
		"pop r15					\n\t" \
		"pop r14					\n\t" \
		"pop r13					\n\t" \
		"pop r12					\n\t" \
		"pop r11					\n\t" \
		"pop r10					\n\t" \
		"pop r9						\n\t" \
		"pop r8						\n\t" \
		"pop r7						\n\t" \
		"pop r6						\n\t" \
		"pop r5						\n\t" \
		"pop r4						\n\t" \
		"pop r3						\n\t" \
		"pop r2						\n\t" \
		"pop r1						\n\t" \
		"pop r0						\n\t" \
		"out %[_SREG_], r0			\n\t" \
		"pop r0						\n\t" \
		";---------------------------\n\t"
		:
		:	[_SREG_] "i" _SFR_IO_ADDR(hal_statusReg),
			[_SPL_] "i" _SFR_IO_ADDR(SPL),
			[_SPH_] "i" _SFR_IO_ADDR(SPH)
	);
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

static inline void kernel_swapTasks(volatile struct kTaskStruct_t * taskA, volatile struct kTaskStruct_t * taskB)
{
	struct kTaskStruct_t taskTemp = *taskA;
	*taskA = *taskB;
	*taskB = taskTemp;
}

static void kernel_sortTaskList() //Bubble sort
{
	uint8_t swapFlag = 0;
	
	for(int i = 0; i < kTaskIndex; i++){
		swapFlag = 0;
		for(int j = 0; j < kTaskIndex-i; j++){
			if(kTaskList[j].priority > kTaskList[j+1].priority){
				swapFlag = 1;
				kernel_swapTasks(&kTaskList[j], &kTaskList[j+1]); //Bruh
			}
		}
		if(!swapFlag) break;
	}
	return;
}

kTaskHandle_t kernel_createTask(kTask_t t_pointer, uint16_t t_stackSize, kTaskPriority_t t_priority, kTaskType_t t_type, uint16_t t_execTime)
{
	if(t_pointer == NULL) return NULL;
	
	if(kStackUsage + t_stackSize >= KERNEL_STACK_SIZE)
		return NULL;
	kStackUsage += t_stackSize;
		
	uint8_t* stackPointer = (uint8_t*)&kernelStack[KERNEL_STACK_SIZE-1] - kStackUsage;
	if(stackPointer == NULL) return NULL;
	
	stackPointer[0] = (uint16_t)t_pointer >> 8;
	stackPointer[-1] = (uint16_t)t_pointer & 0xFF;
	stackPointer[-2] = 0;
	stackPointer[-3] = 0x80;
	
	for (int i = -4; i > -34; i--)
		stackPointer[i] = 0;
	
	kTaskList[kTaskIndex].stackPtr = stackPointer - 36;
	kTaskList[kTaskIndex].stackSize = t_stackSize;
	kTaskList[kTaskIndex].priority = t_priority;
	kTaskList[kTaskIndex].taskPtr = t_pointer;
	kTaskList[kTaskIndex].stackBegin = stackPointer - 36;
	kTaskList[kTaskIndex].status = KSTATE_READY;
	kTaskList[kTaskIndex].type = t_type;
	kTaskList[kTaskIndex].execTime = t_execTime;
	kTaskList[kTaskIndex].execTimeMax = t_execTime;
	kTaskList[kTaskIndex].pid = kGlobalPid;
	
	kernel_sortTaskList(); //Bruh
	
	kGlobalPid++;
	kTaskIndex++;

	return &kTaskList[kTaskIndex];
}

static void kernel_taskSwitch()
{
	kernel_saveContext();
	if(hal_checkBit_m(kflags, KFLAG_CSW_ALLOWED)){
		kernel_switchContext();
	}
	kernel_restoreContext();
}


static inline void kernel_taskManager()
{
	static uint8_t kCurrentTaskIdx = 0;
	for(; kCurrentTaskIdx < kTaskIndex; kCurrentTaskIdx++){
		if(kTaskList[kCurrentTaskIdx].status == KSTATE_READY && kTaskList[kCurrentTaskIdx].stackPtr != NULL){
			kNextTask = &kTaskList[kCurrentTaskIdx];
			break;
		}
	}
	if(kCurrentTaskIdx == kTaskIndex) {
		kCurrentTaskIdx = 0;
	}
	for(int i = 0; i < kTaskIndex; i++){
		if(kTaskList[i].status == KSTATE_SUSPENDED){
			kTaskList[i].execTime = kTaskList[i].execTimeMax;
			kTaskList[i].status = KSTATE_READY;
		}
	}
}

static void kernel_tick()  __attribute__ ( ( naked ) );

static void kernel_tick()
{
	if(kCurrentTask -> execTime != 0){
		kCurrentTask -> execTime -= 1;
		kCurrentTask -> status = KSTATE_READY;
	}
	else {
		kCurrentTask -> status = KSTATE_SUSPENDED;
	}
	kernel_taskManager();
	kernel_ret();
}

uint8_t kernel_init()
{
	hal_uart_init(12);
	kernel_createTask(kernel_idle, 100, KPRIO_NONE, KTASK_DEFAULT, 15);
	hal_setupSystemTimer();
	hal_startSystemTimer();
	kernel_enableContextSwitch_m();
	hal_enableInterrupts();
	debug_puts(L_NONE, PSTR("[kernel] starting up task manager\r\n"));
	kernel_taskManager();
	return 0;
}

inline static void kernel_ISREnter()
{
	kInterruptDepth++;
}

inline static void kernel_ISRExit()
{
	if(kInterruptDepth) kInterruptDepth--;
}

ISR(HAL_TIMER_INTERRUPT_vect, ISR_NAKED)
{
	//kernel_taskService();
	kernel_tick();
	kernel_taskSwitch();
	kernel_reti();
}
