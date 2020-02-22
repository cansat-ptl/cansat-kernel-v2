/*
 * timers.h
 *
 * Created: 22.02.2020 20:56:28
 *  Author: Admin
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

uint8_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_delay);
uint8_t kernel_removeTimer(kTimerISR_t t_pointer);
void kernel_timerService();

#endif /* TIMERS_H_ */