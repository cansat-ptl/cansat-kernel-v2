/*
 * logging.c
 *
 * Created: 05.03.2020 20:02:32
 *  Author: Admin
 */ 

#include <boot/boot.h>
#include <dev/basic/uart.h>

static char buffer[128];

void boot_logMessage(uint8_t level, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	
	vsprintf(buffer, format, args);
	basicUart_puts(buffer);
	
	va_end(args);
}

void boot_logMessage_p(uint8_t level, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	
	vsprintf_P(buffer, format, args);
	basicUart_puts(buffer);
	
	va_end(args);
}