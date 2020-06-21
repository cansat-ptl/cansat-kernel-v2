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

kStackPtr_t platform_prepareStackFrame(kStackPtr_t regionPointer, kStackSize_t stackSize, kTask_t taskPointer, void* args)
{
	//TODO: 3-byte PC support
	regionPointer += stackSize-1;
	regionPointer[0] = (uint16_t)kernel_taskReturnHook & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	regionPointer[-1] = (uint16_t)kernel_taskReturnHook >> 8;
	regionPointer[-2] = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	regionPointer[-3] = (uint16_t)taskPointer >> 8;	// Upper 8 bits
	regionPointer[-4] = 0;							// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	regionPointer[-5] = 0x80;						// SREG initial value - interrupts enabled
	
	for (kIterator_t i = CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET; i > (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET); i--)
		regionPointer[i] = 0;						// R1-R31 initial values
	
	regionPointer[CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET - 23] = (uint16_t)args & 0xFF;
	regionPointer[CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET - 24] = (uint16_t)args >> 8;
	
	return regionPointer + (CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET + CFG_KERNEL_STACK_FRAME_END_OFFSET);
}