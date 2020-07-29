/*
 * timers.h
 *
 * Created: 13.02.2020 19:41:47
 *  Author: Admin
 */ 


#ifndef TIMERS_INTERNAL_H_
#define TIMERS_INTERNAL_H_

#include <stdint.h>

void platform_stopTimer1A();
void platform_startTimer1A();
void platform_setupTimer1A(uint8_t prescaler);
void platform_stopTimer0();
void platform_startTimer0();
void platform_setupTimer0(uint8_t prescaler);

#endif /* TIMERS_H_ */