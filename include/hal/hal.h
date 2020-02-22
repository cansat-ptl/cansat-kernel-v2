/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 

#ifndef HAL_H_
#define HAL_H_

#define HAL_MOD_VER "0.2.2-staging"
#define HAL_MOD_TIMESTAMP __TIMESTAMP__

#include <types.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>
#include <platform/platform.h>

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

#define hal_SET_BIT(x,y) x |= (1 << y)
#define hal_CLEAR_BIT(x,y) x &= ~(1 << y)
#define hal_CHECK_BIT(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define hal_SWITCH_BIT(x,y) (x ^= (1 << y))
#define hal_WRITE_BIT(x,y,z) x ^= (-1 * z ^ x) & (1 << y);

#ifndef hal_UART_INIT
	#define hal_UART_INIT(ubrr) basicUart_init(ubrr)
#endif

#ifndef hal_UART_PUTC
	#define hal_UART_PUTC(c) basicUart_putc(c)
#endif

#ifndef hal_UART_PUTS
	#define hal_UART_PUTS(s) basicUart_puts(s)
#endif

#ifndef hal_UART_ENABLE_RX_INT
	#define hal_UART_ENABLE_RX_INT() basicUart_enableInterruptsRX()
#endif

#ifndef hal_UART_DISABLE_RX_INT
	#define hal_UART_DISABLE_RX_INT(s) basicUart_disableInterruptsRX()
#endif

#endif /* HAL_H_ */