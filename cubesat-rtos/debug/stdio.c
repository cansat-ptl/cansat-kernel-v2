/*
 * stdio.c
 *
 * Created: 23.04.2020 13:33:02
 *  Author: Admin
 */

#include <kernel/kernel.h>
#include <kernel/hal/hal.h>
#include <stdio.h>

static int _uart_putc(char c, FILE *stream);

static FILE _dbgstdout = FDEV_SETUP_STREAM(_uart_putc, NULL, _FDEV_SETUP_WRITE);

static int _uart_putc(char c, FILE *stream)
{
	//if (c == '\n') hal_UART_PUTC('\r');
	hal_UART_PUTC(c);

	return 0;
}

void debug_init()
{
	stdout = &_dbgstdout;
}