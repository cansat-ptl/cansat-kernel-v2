/*
 * logging.c
 *
 * Created: 05.03.2020 20:02:32
 *  Author: Admin
 */ 

#include <boot/boot.h>
#include <dev/basic/uart.h>

static char buffer[128];
uint_farptr_t format_ptr = 0;
uint_farptr_t fptr = 0;

void boot_logMessage(uint8_t level, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	
	vsprintf(buffer, format, args);
	basicUart_puts(buffer);
	
	va_end(args);
}

void boot_logMessage_p(uint8_t level, uint_farptr_t format_p, ...)
{
	char fmt[128];
	
	va_list args;
	va_start(args, format_p);
	
	strcpy_PF(fmt, format_p);
	vsprintf(buffer, fmt, args);
	basicUart_puts(buffer);
	
	va_end(args);
}