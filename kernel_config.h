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

	//Task manager settings
	#define CFG_TASK_STACK_SAFETY_MARGIN 16
	#define CFG_KERNEL_RESERVED_MEMORY 300
	#define CFG_KERNEL_STACK_SAFETY_MARGIN 16
	#define CFG_MIN_STACK_SIZE 40

	#define CFG_ALLOW_INTERRUPTS_IN_ATOMIC_SECTIONS 0
	#define CFG_ENABLE_MEMORY_PROTETCTION 0

	#define CFG_NUMBER_OF_PRIORITIES 8

	#define CFG_TICKS_PER_TASK 5

	#define CFG_HEAP_SIZE 2500 //TODO: move to config
	#define CFG_PLATFORM_BYTE_ALIGNMENT 1
	#define CFG_PLATFORM_BYTE_ALIGNMENT_MASK 0x0000
	#define CFG_MIN_BLOCK_SIZE 4

	//Timer settings
	#define CFG_MAX_TIMER_COUNT 3
	#define CFG_TICKRATE_MS 1

	//Debug logging options
	#define CFG_LOGGING 1
	#define CFG_VERBOSE 0
	#define CFG_PROFILING 0
	#define CFG_DEBUG_PRINT_STAGES 1
	#define CFG_DEBUG_PRINT_LEVELS 1

	#define CFG_SYSTEMIO_MAX_FILE_DESCRIPTORS 10

	//HAL timer selection - uncomment the timer to select it
	#define CFG_HAL_USE_TIMER0
	//#define HAL_USE_TIMER1A
#endif
#endif /* KERNEL_CONFIG_H_ */