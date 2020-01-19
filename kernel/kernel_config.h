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

	//Kernel modules - set to 0 to disable
	#define KERNEL_SD_MODULE 0
	#define KERNEL_WDT_MODULE 0
	#define KERNEL_UTIL_MODULE 0
	#define KERNEL_DEBUG_MODULE 1
	#define KERNEL_CLI_MODULE 0
	#define KERNEL_TIMER_MODULE 0

	//Task manager settings
	#define TASK_STACK_SIZE 900
	#define TASK_STACK_SAFETY_MARGIN 16
	#define KERNEL_STACK_SIZE 100
	#define KERNEL_STACK_SAFETY_MARGIN 32
	#define MAX_TASK_COUNT 5
	#define MIN_STACK_SIZE 40 //Due to AVR platform limitations, it is NOT RECOMMENDED to use values < 40, or bad things may happen
	#define KERNEL_STACK_FRAME_REGISTER_OFFSET -4  //Memory offset, from which General Purpose Registers are stored during stack initialization
	
	//Timer settings
	#define MAX_TIMER_COUNT 3
	#define KERNEL_TIMER_PRESCALER 3
	#define TICKRATE_MS 1
	
	/*             Prescaler values             */
	/* 0 - timer disabled                       */
	/* 1 - no prescaling (F_TIMER = F_CPU)      */
	/* 2 - F_CPU / 8                            */
	/* 3 - F_CPU / 64                           */
	/* 4 - F_CPU / 256                          */
	/* 5 - F_CPU / 1024                         */
	/* 6 & 7 - external clock (datasheet)       */
	
	//Debug logging options
	#define LOGGING 1
	#define VERBOSE 0
	#define PROFILING 1
	
	//CLI options
	#define RX0_BUFFER_SIZE 64
	#define MAX_CMD_WORD_SIZE 16

	//HAL uart settings - comment these lines to use built-in uart driver, or specify your own uart functions
	//#define hal_uart_init(ubrr) uart0_init(ubrr)
	//#define hal_uart_putc(c) uart0_putc(c)
	//#define hal_uart_puts(s) uart0_puts(s)
	//#define hal_uart_enableInterruptsRX() rx0_enableInterrupts()
	//#define hal_uart_disableInterruptsRX() rx0_disableInterrupts()
	
	//HAL timer selection - uncomment the timer to select it
	#define HAL_USE_TIMER0
	//#define HAL_USE_TIMER1A
#endif
#endif /* KERNEL_CONFIG_H_ */