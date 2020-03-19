/*
 * utils.c
 *
 * Created: 19.03.2020 19:39:57
 *  Author: Admin
 */

#include <boot/boot.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>

uint8_t boot_convertAsciiToNumeric(unsigned char c)
{
	if (c >= '0' && c <= '9') {
		return (c - 0x30);
	}
	else if (c >= 'a' && c <= 'f') {
		return (c - 87);
	}
	else if (c >= 'A' && c <= 'F') {
		return (c - 55);
	}
	return 0xFF;
}