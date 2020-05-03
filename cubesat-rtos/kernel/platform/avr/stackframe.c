/*
 * stackalloc.c
 *
 * Created: 13.02.2020 19:22:52
 *  Author: Admin
 */ 

#include <kernel/kernel.h>
#include <kernel/hal/hal.h>
#include <kernel/debug.h>
#include <kernel/platform/platform.h>
#include <kernel/types.h>
#include "../kernel_config.h"

void kernel_taskReturnHook();
void kernel_stackCorruptionHook();

//TODO: return codes
uint8_t platform_prepareStackFrame(kStackPtr_t stackPointer, kTask_t taskPointer, void* args)
{
	if (stackPointer == NULL) return 0;			// Return null if memory sp is null (how could this happen?)
	
	//TODO: 3-byte PC support
	stackPointer[0] = (uint16_t)kernel_taskReturnHook & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	stackPointer[-1] = (uint16_t)kernel_taskReturnHook >> 8;
	stackPointer[-2] = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	stackPointer[-3] = (uint16_t)taskPointer >> 8;	// Upper 8 bits
	stackPointer[-4] = 0;							// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	stackPointer[-5] = 0x80;						// SREG initial value - interrupts enabled
	
	for (int16_t i = CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET; i > (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET); i--)
		stackPointer[i] = 0;						// R1-R31 initial values
	
	stackPointer[CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET - 23] = (uint16_t)args & 0xFF;
	stackPointer[CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET - 24] = (uint16_t)args >> 8;
	
	return 0;
}

void platform_handleStackCorruption()
{
	kTaskHandle_t handle = kernel_getCurrentTaskHandle();
	kStackPtr_t stackPointer = handle -> stackBegin;
	platform_prepareStackFrame(stackPointer, kernel_stackCorruptionHook, NULL);
	handle -> stackPtr = stackPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET+CFG_KERNEL_STACK_FRAME_END_OFFSET);
}