/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 

#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.0.2-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <types.h>
#include <hal/hal.h>
#include <threads/threads.h>
#include <kernel/control.h>
#include <kernel/tasks.h>
#include <kernel/memory.h>
#include <kernel/debug.h>
#include <platform/platform.h>
#include "../kernel_config.h"

uint8_t kernel_startScheduler();
void kernel_init();

uint8_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_period);
uint8_t kernel_removeTimer(kTimerISR_t t_pointer);
void kernel_timerService();


#endif /* KERNEL_H_ */