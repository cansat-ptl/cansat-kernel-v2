/*
 * kernel.h
 *
 * Created: 29.07.2020 13:32:16
 *  Author: Admin
 */ 


#ifndef KERNEL_INTERNAL_H_
#define KERNEL_INTERNAL_H_

#include <stdint.h>
#include <kernel/ktypes.h>

uint8_t kernel_getSystemStatus();
void kernel_setSystemStatus(uint8_t status);

void kernel_panic(const char * message);

void kernel_taskReturnHook();
void kernel_stackCorruptionHook(kTaskHandle_t task);

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);

uint64_t kernel_getUptime();

#endif /* KERNEL_H_ */