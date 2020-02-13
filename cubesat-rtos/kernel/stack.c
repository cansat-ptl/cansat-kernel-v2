/*
 * stack.c
 *
 * Created: 13.02.2020 22:16:38
 *  Author: Admin
 */ 

#include <kernel.h>

static volatile uint8_t kernelStack[TASK_STACK_SIZE + KERNEL_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN];
static volatile kStackSize_t kUserTaskStackUsage = 0;
static volatile kStackSize_t kSystemStackUsage = 0;

kStackPtr_t kernel_getStackPtr()
{
	return kernelStack;
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
	kStackPtr_t stackPointer = NULL;
	
	if (taskType != KTASK_SYSTEM) {
		if (kUserTaskStackUsage + taskStackSize + TASK_STACK_SAFETY_MARGIN >= TASK_STACK_SIZE) return NULL;
		
		stackPointer = (&kernelStack[TASK_STACK_SIZE-1] - kUserTaskStackUsage);  // Calculating task stack pointer
		kUserTaskStackUsage += taskStackSize + TASK_STACK_SAFETY_MARGIN;	// Incrementing stack usage value, 16 bytes for memory protection region
		
		kernel_prepareMemoryBarrier((stackPointer - taskStackSize), TASK_STACK_SAFETY_MARGIN, 0xFE);
	}
	else {
		if (kSystemStackUsage + taskStackSize + TASK_STACK_SAFETY_MARGIN >= KERNEL_STACK_SIZE) return NULL;
		stackPointer = (&kernelStack[(TASK_STACK_SIZE + KERNEL_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN)-1] - kSystemStackUsage);  // Calculating task stack pointer
		kSystemStackUsage += taskStackSize + TASK_STACK_SAFETY_MARGIN;
	}
	
	hal_prepareStackFrame(stackPointer, startupPointer, taskStackSize);
	
	return stackPointer;
}