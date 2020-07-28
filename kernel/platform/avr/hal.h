/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */

#ifndef HAL_H_
#define HAL_H_

#include <kernel/types.h>

#include "../hal_pins.h"
#include "../kernel_config.h"

#ifdef HAL_USE_TIMER0
	#define HAL_TIMER_INTERRUPT_vect TIMER0_COMP_vect
	#define hal_SETUP_SYSTEM_TIMER() hal_setupTimer0(KERNEL_TIMER_PRESCALER)
	#define hal_START_SYSTEM_TIMER() hal_startTimer0()
	#define hal_STOP_SYSTEM_TIMER() hal_stopTimer0()
#endif
#ifdef HAL_USE_TIMER1A
	#define HAL_TIMER_INTERRUPT_vect TIMER1_COMPA_vect
	#define hal_SETUP_SYSTEM_TIMER() hal_setupTimer1A(KERNEL_TIMER_PRESCALER)
	#define hal_START_SYSTEM_TIMER() hal_startTimer1A()
	#define hal_STOP_SYSTEM_TIMER() hal_stopTimer1A()
#endif

#ifndef hal_REBOOT
	#define hal_REBOOT() while (1){;}
#endif

#endif /* HAL_H_ */