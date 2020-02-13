/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 

#ifndef HAL_H_
#define HAL_H_

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define HAL_MOD_VER "0.2.2-staging"
#define HAL_MOD_TIMESTAMP __TIMESTAMP__

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/wdt.h>

#include <types.h>
#include <hal/gpio.h>
#include <hal/timers.h>
#include <hal/uart.h>
#include "../kernel_config.h"
#include "../hal_pins.h"

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

#define hal_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define hal_ENABLE_INTERRUPTS() asm volatile ("sei"::)

#define hal_STATUS_REG SREG
#define hal_NOP() asm volatile ("nop"::)
#define hal_DELAY_MS(x) _delay_ms(x);

#define hal_SET_BIT(x,y) x |= (1 << y)
#define hal_CLEAR_BIT(x,y) x &= ~(1 << y)
#define hal_CHECK_BIT(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define hal_SWITCH_BIT(x,y) (x ^= (1 << y))
#define hal_WRITE_BIT(x,y,z) x ^= (-1 * z ^ x) & (1 << y);

uint8_t hal_prepareStackFrame(kStackPtr_t regionPointer, kTask_t taskPointer, kStackSize_t stackSize);

#endif /* HAL_H_ */