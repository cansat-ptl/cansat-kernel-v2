/*
 * stack.c
 *
 * Created: 13.02.2020 22:16:38
 *  Author: Admin
 */

#include <kernel.h>

static volatile uint8_t kernelMemory[CFG_TASK_STACK_SIZE + CFG_KERNEL_STACK_SIZE + CFG_KERNEL_STACK_SAFETY_MARGIN];
static volatile kStackSize_t kUserTaskStackUsage = 0;
static volatile kStackSize_t kSystemStackUsage = 0;

void platform_handleStackCorruption();

kStackPtr_t kernel_getStackPtr()
{
	return kernelMemory;
}

kStackSize_t kernel_getUserTaskStackUsage()
{
	return kUserTaskStackUsage;
}

kStackSize_t kernel_getSystemTaskStackUsage()
{
	return kSystemStackUsage;
}

void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler)
{
	for (int16_t i = 0; i > -size; i--)
		sptr[i] = filler;
}

kStackPtr_t kernel_setupTaskStack(kTask_t startupPointer, kStackSize_t taskStackSize, kTaskType_t taskType)
{
	/* Preparing initial stack frame - DARK MAGIC, DO NOT TOUCH */
	uint8_t sreg = threads_startAtomicOperation();
	kStackPtr_t stackPointer = NULL;

	if (taskType != KTASK_SYSTEM) {
		if (kUserTaskStackUsage + taskStackSize + CFG_TASK_STACK_SAFETY_MARGIN >= CFG_TASK_STACK_SIZE) {
			threads_endAtomicOperation(sreg);
			return NULL;
		}
		stackPointer = (&kernelMemory[CFG_TASK_STACK_SIZE-1] - kUserTaskStackUsage);  // Calculating task stack pointer
		kUserTaskStackUsage += taskStackSize + CFG_TASK_STACK_SAFETY_MARGIN;	// Incrementing stack usage value, 16 bytes for memory protection region

		kernel_prepareMemoryBarrier((stackPointer - taskStackSize), CFG_TASK_STACK_SAFETY_MARGIN, 0xFE);
	}
	else {
		if (kSystemStackUsage + taskStackSize + CFG_TASK_STACK_SAFETY_MARGIN >= CFG_KERNEL_STACK_SIZE) {
			threads_endAtomicOperation(sreg);
			return NULL;
		}
		stackPointer = (&kernelMemory[(CFG_TASK_STACK_SIZE + CFG_KERNEL_STACK_SIZE + CFG_KERNEL_STACK_SAFETY_MARGIN)-1] - kSystemStackUsage);  // Calculating task stack pointer
		kSystemStackUsage += taskStackSize + CFG_TASK_STACK_SAFETY_MARGIN;
	}

	platform_prepareStackFrame(stackPointer, startupPointer);

	threads_endAtomicOperation(sreg);
	return stackPointer;
}

uint8_t kernel_checkStackProtectionRegion(kTaskHandle_t checkedTask)
{
	if (checkedTask == NULL) return 1;

	uint8_t sreg = threads_startAtomicOperation();

	if (checkedTask -> type != KTASK_SYSTEM) {
		kStackPtr_t stackEnd = checkedTask -> stackBegin - checkedTask -> stackSize - CFG_TASK_STACK_SAFETY_MARGIN;

		for (int16_t i = 1; i < CFG_TASK_STACK_SAFETY_MARGIN+1; i++) {
			if (stackEnd[i] != 0xFE) {
				debug_puts(L_NONE, PSTR("kernel: Stack corruption detected\r\n"));
				platform_handleStackCorruption();
			}
		}
	}

	threads_endAtomicOperation(sreg);
	return 0;
}