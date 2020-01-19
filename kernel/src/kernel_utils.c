/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"

extern volatile uint16_t __e_time;

uint64_t kernel_getUptime()
{
	return (uint64_t)__e_time;
}