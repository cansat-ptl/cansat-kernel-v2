/*
 * stackalloc.c
 *
 * Created: 13.02.2020 19:22:52
 *  Author: Admin
 */ 
#include <hal/hal.h>
#include <kernel.h>
#include <types.h>
#include "../kernel_config.h"
//TODO: return codes
uint8_t hal_prepareStackFrame(kStackPtr_t regionPointer, kTask_t taskPointer, kStackSize_t stackSize)
{
	if (regionPointer == NULL) return 0;			// Return null if memory sp is null (how could this happen?)
	
	regionPointer[0] = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	regionPointer[-1] = (uint16_t)taskPointer >> 8;	// Upper 8 bits, TODO: 3 byte PC support
	regionPointer[-2] = 0;							// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	regionPointer[-3] = 0x80;						// SREG initial value - interrupts enabled
	
	for (int16_t i = KERNEL_STACK_FRAME_REGISTER_OFFSET; i > (KERNEL_STACK_FRAME_REGISTER_OFFSET-31); i--)
		regionPointer[i] = 0;						// R1-R31 initial values
	return 0;
}