/*
 * utils.c
 *
 * Created: 23.06.2019 10:27:23
 *  Author: ThePetrovich
 */ 
#include "utils.h"
#include <kernel/ktypes.h>

uint8_t util_strCompare(char * a, char * b, uint8_t len)
{
	for (kIterator_t i = 0; i < len; i++) {
		if (a[i] != b[i]) return 0;
	}
	return 1;
}

uint8_t util_convertAsciiToNumeric(char c)
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