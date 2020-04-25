/*
 * timers.h
 *
 * Created: 22.02.2020 20:56:28
 *  Author: Admin
 */ 


#ifndef SOFTWARETIMERS_H_
#define SOFTWARETTIMERS_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

kTimerHandle_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_period);
void kernel_removeTimer(kTimerHandle_t handle);
void kernel_timerService();

#endif /* TIMERS_H_ */