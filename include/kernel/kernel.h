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

#include <kernel/types.h>
#include <kernel/hal/hal.h>
#include <kernel/threads/threads.h>
#include <kernel/control.h>
#include <kernel/tasks.h>
#include <kernel/softtimers.h>
#include <kernel/memory.h>
#include <kernel/debug.h>
#include <kernel/utils/utils.h>
#include <kernel/utils/time.h>
#include <kernel/platform/platform.h>
#include "../kernel_config.h"

uint8_t kernel_startScheduler();
void kernel_preinit();

#endif /* KERNEL_H_ */