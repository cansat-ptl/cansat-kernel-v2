/*
 * stdio.c
 *
 * Created: 23.04.2020 13:33:02
 *  Author: Admin
 */

#include "hal.h"
#include <stdio.h>

//TODO: remove stdio

static int _uart_putc(char c, FILE *stream);
static int _uart_getc(FILE *stream);

static FILE _dbgstdout = FDEV_SETUP_STREAM(_uart_putc, NULL, _FDEV_SETUP_WRITE);
static FILE _dbgstdin = FDEV_SETUP_STREAM(NULL, _uart_getc, _FDEV_SETUP_READ);
static FILE _dbgstderr = FDEV_SETUP_STREAM(_uart_putc, NULL, _FDEV_SETUP_WRITE);

static int _uart_putc(char c, FILE *stream)
{
	//if (c == '\n') hal_UART_PUTC('\r');
	hal_UART_PUTC(c);

	return 0;
}

static int _uart_getc(FILE *stream)
{
	//if (c == '\n') hal_UART_PUTC('\r');
	//TODO: implement getc()
	return 0;
}

void debug_setStdout(FILE* desc)
{
	stdout = desc;
}

void debug_setStdin(FILE* desc)
{
	stdin = desc;
}

void debug_setStderr(FILE* desc)
{
	stderr = desc;
}

FILE* debug_getGlobalStdout()
{
	return &_dbgstdout;
}

FILE* debug_getGlobalStdin()
{
	return &_dbgstdin;
}

FILE* debug_getGlobalStderr()
{
	return &_dbgstderr;
}

void debug_init()
{
	basicUart_init(24);
	debug_setStdin(&_dbgstdin);
	debug_setStdout(&_dbgstdout);
	debug_setStderr(&_dbgstderr);
}