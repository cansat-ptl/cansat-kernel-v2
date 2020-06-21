/*
 * utils.h
 *
 * Created: 21.06.2020 20:19:02
 *  Author: Admin
 */


#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

struct kSystemTimeStruct_t
{
	uint16_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};

#endif /* UTILS_H_ */