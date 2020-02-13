/*
 * time.h
 *
 * Created: 13.02.2020 22:45:07
 *  Author: Admin
 */ 


#ifndef TIME_H_
#define TIME_H_

#include <types.h>

void time_updateSystemTime();
uint16_t time_getMilliseconds();
uint8_t time_getSeconds();
uint8_t time_getMinutes();
uint8_t time_getHours();
uint16_t time_getDays();

#endif /* TIME_H_ */