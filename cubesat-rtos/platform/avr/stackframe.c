/*
 * stackalloc.c
 *
 * Created: 13.02.2020 19:22:52
 *  Author: Admin
 */ 

#include <kernel.h>
#include <hal/hal.h>
#include <kernel/debug.h>
#include <platform/platform.h>
#include <types.h>
#include "../kernel_config.h"

//TODO: return codes
uint8_t platform_prepareStackFrame(kStackPtr_t stackPointer, kTask_t taskPointer)
{
	if (stackPointer == NULL) return 0;			// Return null if memory sp is null (how could this happen?)
	
	//TODO: 3-byte PC support
	stackPointer[0] = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	stackPointer[-1] = (uint16_t)taskPointer >> 8;	// Upper 8 bits
	stackPointer[-2] = 0;							// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	stackPointer[-3] = 0x80;						// SREG initial value - interrupts enabled
	
	for (int16_t i = CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET; i > (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET-31); i--)
		stackPointer[i] = 0;						// R1-R31 initial values
	return 0;
}

void platform_stackCorruptionHook()
{
	uint8_t sreg = threads_startAtomicOperation();
	
	kTaskHandle_t handle = kernel_getCurrentTaskHandle();
	kTaskHandle_t taskList = kernel_getTaskListPtr();
	uint8_t taskIndex = kernel_getTaskListIndex();
	
	handle -> state = KSTATE_UNINIT;
	debug_puts(L_NONE, PSTR("kernel: Executing task corruption hook\r\n")); //TODO: debug information
	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i].lock -> owner == handle) {
			taskList[i].state = KSTATE_READY;
			taskList[i].lock -> owner = NULL;
			taskList[i].lock -> lockCount = 0;
		}
	}
	threads_endAtomicOperation(sreg);
	kernel_yield(0);
}

void platform_handleStackCorruption()
{
	kTaskHandle_t handle = kernel_getCurrentTaskHandle();
	kStackPtr_t stackPointer = handle -> stackBegin;
	platform_prepareStackFrame(stackPointer, platform_stackCorruptionHook);
	handle -> stackPtr = stackPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET-CFG_KERNEL_STACK_FRAME_END_OFFSET);
	
	
}