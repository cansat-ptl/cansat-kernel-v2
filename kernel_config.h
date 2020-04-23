/*
 * kernel_config.h
 *
 * Created: 30.07.2019 22:31:10
 *  Author: ThePetrovich
 */

#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#ifndef KERNELconfig
#define KERNELconfig
	//Custom includes
	//#include "../drivers/interfaces/uart.h"

	//Task manager settings
	#define CFG_TASK_STACK_SIZE 1400
	#define CFG_TASK_STACK_SAFETY_MARGIN 16
	#define CFG_KERNEL_STACK_SIZE 200
	#define CFG_KERNEL_RESERVED_MEMORY 100
	#define CFG_KERNEL_STACK_SAFETY_MARGIN 32
	#define CFG_MAX_TASK_COUNT 10
	#define CFG_MIN_STACK_SIZE 40 //Due to AVR platform limitations, it is NOT RECOMMENDED to use values < 40, or bad things may happen
	#define CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET -6  //Memory offset, from which General Purpose Registers are stored during stack initialization
	#define CFG_KERNEL_STACK_FRAME_END_OFFSET -31
	#define CFG_TICKS_PER_TASK 5

	//Platform-dependent settings
	#define CFG_USE_3BYTE_PC 0 //AVR specific option
	#define CFG_USE_RAMP_REGISTERS 0 //AVR specific option

	#define CFG_NO_INTERRUPTS_IN_CRITICAL_SECTIONS 0
	#define CFG_ENABLE_MEMORY_PROTETCTION 1

	//Timer settings
	#define CFG_MAX_TIMER_COUNT 3
	#define CFG_KERNEL_TIMER_PRESCALER 3
	#define CFG_TICKRATE_MS 1

	/*             Prescaler values             */
	/* 0 - timer disabled                       */
	/* 1 - no prescaling (F_TIMER = F_CPU)      */
	/* 2 - F_CPU / 8                            */
	/* 3 - F_CPU / 64                           */
	/* 4 - F_CPU / 256                          */
	/* 5 - F_CPU / 1024                         */
	/* 6 & 7 - external clock (datasheet)       */

	//Debug logging options
	#define CFG_LOGGING 1
	#define CFG_VERBOSE 0
	#define CFG_PROFILING 1

	//CLI options
	#define CFG_RX0_BUFFER_SIZE 64
	#define CFG_MAX_CMD_WORD_SIZE 16

	//HAL timer selection - uncomment the timer to select it
	#define CFG_HAL_USE_TIMER0
	//#define HAL_USE_TIMER1A
#endif
#endif /* KERNEL_CONFIG_H_ */