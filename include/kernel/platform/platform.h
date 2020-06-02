/*
 * platform.h
 *
 * Created: 15.02.2020 19:30:29
 *  Author: Admin
 */


#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <kernel/platform/avr/avr.h>
#include <kernel/platform/avr/timers.h>

#define CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET -6  //Memory offset, from which General Purpose Registers are stored during stack initialization
#define CFG_KERNEL_STACK_FRAME_END_OFFSET -31 //Last GPR position on stack
#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_USE_3BYTE_PC 0 //AVR specific option
#define CFG_USE_RAMP_REGISTERS 0 //AVR specific option

/*             Prescaler values             */
/* 0 - timer disabled                       */
/* 1 - no prescaling (F_TIMER = F_CPU)      */
/* 2 - F_CPU / 8                            */
/* 3 - F_CPU / 64                           */
/* 4 - F_CPU / 256                          */
/* 5 - F_CPU / 1024                         */
/* 6 & 7 - external clock (datasheet)       */

#define CFG_KERNEL_TIMER_PRESCALER 3

#endif /* PLATFORM_H_ */