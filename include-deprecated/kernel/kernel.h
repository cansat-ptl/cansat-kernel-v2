/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.2.1-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../kernel_config.h"

#include <kernel/types.h>
#include <kernel/hal/hal.h>
#include <kernel/threads/threads.h>
#include <kernel/tasks.h>
#include <kernel/softtimers.h>
#include <kernel/memory.h>
#include <kernel/debug.h>
#include <kernel/utils/utils.h>
#include <kernel/utils/time.h>
#include <kernel/platform/platform.h>

void kernel_startup();

void kernel_panic();

kStackPtr_t kernel_getStackPtr();
kStackSize_t kernel_getUserTaskStackUsage();
kStackSize_t kernel_getSystemTaskStackUsage();

void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler);
kReturnValue_t kernel_checkStackProtectionRegion(kTaskHandle_t checkedTask);

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);
uint64_t kernel_getUptime();
void kernel_checkMCUCSR();

uint8_t kernel_getSystemStatus();
void kernel_setSystemStatus(uint8_t status);

kReturnValue_t kernel_startScheduler();
void kernel_preinit();

#endif /* KERNEL_H_ */