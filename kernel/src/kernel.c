/*
 * kernel.c
 *
 * Created: 11.05.2019 21:12:52
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"
#include "../kernel_config.h"

#define kernel_RET() asm volatile ("ret \n\t" ::)
#define kernel_RETI() asm volatile ("reti \n\t" ::)
#define kernel_DISABLE_CONTEXT_SWITCH() hal_CLEAR_BIT(kflags, KFLAG_CSW_ALLOWED)
#define kernel_ENABLE_CONTEXT_SWITCH() hal_SET_BIT(kflags, KFLAG_CSW_ALLOWED)

static volatile struct kTaskStruct_t *kCurrentTask;
static volatile struct kTaskStruct_t *kNextTask;
static volatile uint16_t kflags = 0;
volatile uint64_t __e_time = 0;
static volatile uint8_t kInterruptDepth = 0;
static volatile uint8_t kGlobalPid = 1;
extern uint8_t mcucsr_mirror;

static volatile struct kTaskStruct_t kTaskList[MAX_TASK_COUNT];
static volatile uint8_t kTaskIndex = 0;

static volatile uint8_t kernelStack[TASK_STACK_SIZE + KERNEL_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN];
static volatile uint16_t kUserTaskStackUsage = 0;
static volatile uint16_t kSystemStackUsage = 0;

static volatile uint8_t kCurrentTaskIdx = 0; //TODO: task switch logic

static inline void kernel_switchTask();
static void kernel_tick()  __attribute__ ( ( naked, noinline ));
void kernel_yield()  __attribute__ ( ( naked, noinline ));
void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount);

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


void kernel_enterCriticalSection()
{
	kernel_DISABLE_CONTEXT_SWITCH();
}

void kernel_exitCriticalSection()
{
	kernel_ENABLE_CONTEXT_SWITCH();
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

void kernel_idle() 
{
	while(1) hal_NOP();
}

uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskStatus_t t_state)
{
	if (t_handle == NULL) return ERR_NULLPTR;
	
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
		:	[_SREG_] "i" _SFR_IO_ADDR(hal_STATUS_REG),
			[_SPL_] "i" _SFR_IO_ADDR(SPL),
			[_SPH_] "i" _SFR_IO_ADDR(SPH)
	);
}

static inline void kernel_switchContext()
{
	if (hal_CHECK_BIT(kflags, KFLAG_CSW_ALLOWED)) {
		asm volatile (
			"lds  r26, kNextTask \n\t"	\
			"lds  r27, kNextTask+1 \n\t" \
			"sts  kCurrentTask, r26 \n\t" \
			"sts  kCurrentTask+1, r27 \n\t"
			:
			:
		);	
	}
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
		"sei						\n\t" \
		"pop r0						\n\t" \
		"out %[_SREG_], r0			\n\t" \
		"pop r0						\n\t" \
		";---------------------------\n\t"
		:
		:	[_SREG_] "i" _SFR_IO_ADDR(hal_STATUS_REG),
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

kTaskHandle_t kernel_createTask(kTask_t t_pointer, uint16_t t_stackSize, kTaskPriority_t t_priority, kTaskType_t t_type, uint16_t t_execTime)
{	
	if (t_pointer == NULL) return NULL;

	/* Preparing initial stack frame - DARK MAGIC, DO NOT TOUCH */
	
	kStackPtr_t stackPointer = NULL;
	
	if (t_type != KTASK_SYSTEM) {
		if (kUserTaskStackUsage + t_stackSize + TASK_STACK_SAFETY_MARGIN >= TASK_STACK_SIZE) return NULL;
		
		stackPointer = (&kernelStack[TASK_STACK_SIZE-1] - kUserTaskStackUsage);  // Calculating task stack pointer
		kUserTaskStackUsage += t_stackSize + TASK_STACK_SAFETY_MARGIN;	// Incrementing stack usage value, 16 bytes for memory protection region
		
		for (int16_t i = -t_stackSize; i > (-1*(int16_t)t_stackSize)-TASK_STACK_SAFETY_MARGIN; i--)
		stackPointer[i] = 0xFE;						// Memory protection margin, filled with placeholders
	}
	else {
		if (kSystemStackUsage + t_stackSize + TASK_STACK_SAFETY_MARGIN >= KERNEL_STACK_SIZE) return NULL;
		stackPointer = (&kernelStack[(TASK_STACK_SIZE + KERNEL_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN)-1] - kSystemStackUsage);  // Calculating task stack pointer
		kSystemStackUsage += t_stackSize + TASK_STACK_SAFETY_MARGIN;
	}
	
	if (stackPointer == NULL) return NULL;			// Return null if memory sp is null (how could this happen?)
	
	stackPointer[0] = (uint16_t)t_pointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	stackPointer[-1] = (uint16_t)t_pointer >> 8;	// Upper 8 bits, TODO: 3 byte PC support
	stackPointer[-2] = 0;							// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	stackPointer[-3] = 0x80;						// SREG initial value - interrupts enabled
	
	for (int16_t i = KERNEL_STACK_FRAME_REGISTER_OFFSET; i > (KERNEL_STACK_FRAME_REGISTER_OFFSET-31); i--)
		stackPointer[i] = 0;						// R1-R31 initial values
	
	kTaskList[kTaskIndex].stackPtr = stackPointer + (KERNEL_STACK_FRAME_REGISTER_OFFSET-31);
	kTaskList[kTaskIndex].stackSize = t_stackSize;
	kTaskList[kTaskIndex].priority = t_priority;
	kTaskList[kTaskIndex].taskPtr = t_pointer;
	kTaskList[kTaskIndex].stackBegin = stackPointer;
	kTaskList[kTaskIndex].lock = NULL;
	kTaskList[kTaskIndex].status = KSTATE_READY;
	kTaskList[kTaskIndex].type = t_type;
	kTaskList[kTaskIndex].execTime = t_execTime;
	kTaskList[kTaskIndex].execTimeMax = t_execTime;
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

uint8_t kernel_init()
{
	hal_UART_INIT(12); 
	
	debug_puts(L_NONE, PSTR("[init] kernel: Startup\r\n")); //TODO: re-implement logging switch
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager                      [OK]\r\n"));
	debug_puts(L_NONE, PSTR("[init] kernel: Setting up idle task"));
	
	kStackPtr_t sptr = &kernelStack[(TASK_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN)-1];
	for (int16_t i = 0; i > -KERNEL_STACK_SAFETY_MARGIN; i--)
		sptr[i] = 0xFE;
	
	kTaskHandle_t ct = kernel_createTask(kernel_idle, 64, KPRIO_NONE, KTASK_SYSTEM, 15);
	if (ct == NULL) {
		debug_puts(L_NONE, PSTR("                          [ERR]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: Memory allocation error\r\n"));
		while(1);
	}
	ct -> pid = 0;
	debug_puts(L_NONE, PSTR("                          [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("===========================System=Info===========================\r\n"));
	for (int i = 0; i < kTaskIndex; i++) {
		debug_puts(L_NONE, PSTR("-----------------------------------------------------------------\r\n"));
		debug_logMessage(PGM_ON, L_NONE, PSTR(" Task %2d: stackPtr:                                       0x%04X\r\n"), i, kTaskList[i].stackPtr);
		debug_logMessage(PGM_ON, L_NONE, PSTR("          taskPtr:                                        0x%04X\r\n"), kTaskList[i].taskPtr);
		debug_logMessage(PGM_ON, L_NONE, PSTR("          stackBegin:                                     0x%04X\r\n"), kTaskList[i].stackBegin);
		debug_logMessage(PGM_ON, L_NONE, PSTR("          stackSize:                                  %4d bytes\r\n"), kTaskList[i].stackSize);
		debug_logMessage(PGM_ON, L_NONE, PSTR("          priority:                                         %4d\r\n"), kTaskList[i].priority);
		debug_logMessage(PGM_ON, L_NONE, PSTR("          pid:                                              %4d\r\n"), kTaskList[i].pid);
	}
	debug_puts(L_NONE, PSTR("-----------------------------------------------------------------\r\n"));
	debug_logMessage(PGM_ON, L_NONE, PSTR("Stack usage:                                           %4d bytes \r\n"), kUserTaskStackUsage + kSystemStackUsage);
	debug_logMessage(PGM_ON, L_NONE, PSTR("Stack size:                                            %4d bytes \r\n"), TASK_STACK_SIZE + KERNEL_STACK_SIZE);
	debug_puts(L_NONE, PSTR("=================================================================\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up first task"));
	kCurrentTask = ct;
	debug_puts(L_NONE, PSTR("                        [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Setting up system timer"));
	hal_setupTimer0(3);
	debug_puts(L_NONE, PSTR("                       [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up system timer"));
	hal_startTimer0();
	debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: System startup complete\r\n"));
	//_delay_ms(3000);
	
	debug_puts(L_NONE, PSTR("\x0C"));
	
	hal_ENABLE_INTERRUPTS();
	kernel_ENABLE_CONTEXT_SWITCH();

	return 0;
}

static inline void kernel_switchTask()
{	
	for (int i = 0; i < kTaskIndex; i++) {
		if(kTaskList[i].status == KSTATE_READY && &kTaskList[i] != kCurrentTask) kNextTask = &kTaskList[i];
	}
	
	if (kNextTask != kCurrentTask) kernel_switchContext();
}

void kernel_yield() 
{
	kernel_saveContext();
	kernel_switchTask();
	kernel_restoreContext();
	kernel_RET();
}

static void kernel_tick()
{
	kernel_saveContext();
	__e_time++;
	kernel_switchTask();
	//hal_startTimer0();
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
