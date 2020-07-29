/*
 * kernel.h
 *
 * Created: 21.06.2020 20:22:55
 *  Author: Admin
 */


#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kernel/tasks.h>
#include <kernel/ipc.h>
#include <kernel/threads.h>
#include <kernel/tasks.h>
#include <kernel/timers.h>
#include <kernel/memory.h>

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);

uint64_t kernel_getUptime();
uint8_t kernel_getSystemStatus();

void kernel_startup();

#endif /* KERNEL_H_ */