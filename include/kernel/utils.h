/*
 * utils.h
 *
 * Created: 21.06.2020 20:19:02
 *  Author: Admin
 */


#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

void time_updateSystemTime();
uint16_t time_getMilliseconds();
uint8_t time_getSeconds();
uint8_t time_getMinutes();
uint8_t time_getHours();
uint16_t time_getDays();

#endif /* UTILS_H_ */