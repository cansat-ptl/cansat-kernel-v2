/*
 * hal.h
 *
 * Created: 28.07.2020 21:36:08
 *  Author: Admin
 */ 


#ifndef HAL_H_
#define HAL_H_

#include "uart.h"
#include <avr/pgmspace.h>

#define utils_SET_BIT(x,y) ((x) |= (1u << (y)))
#define utils_CLEAR_BIT(x,y) ((x) &= ~(1u << (y)))
#define utils_CHECK_BIT(x,y) (((0u == ((x) & (1u << (y)))) ? 0u : 1u))
#define hal_SWITCH_BIT(x,y) ((x) ^= (1u << (y)))
#define utils_WRITE_BIT(x,y,z) ((x) = ((x) & ~(1u << (y))) | ((z) << (y)));

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

#ifndef hal_READ_BYTE_FROM_FLASH
#define hal_READ_BYTE_FROM_FLASH(x) pgm_read_byte(x)
#endif

#ifndef hal_READ_WORD_FROM_FLASH
#define hal_READ_WORD_FROM_FLASH(x) pgm_read_word(x)
#endif

#ifndef hal_READ_DWORD_FROM_FLASH
#define hal_READ_DWORD_FROM_FLASH(x) pgm_read_dword(x)
#endif

#ifndef hal_REBOOT
#define hal_REBOOT() while (1){;}
#endif

#endif /* HAL_H_ */