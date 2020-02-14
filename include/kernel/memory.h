/*
 * memory.h
 *
 * Created: 14.02.2020 20:59:16
 *  Author: Admin
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

kStackPtr_t kernel_getStackPtr();
kStackSize_t kernel_getUserTaskStackUsage();
kStackSize_t kernel_getSystemTaskStackUsage();

kStackPtr_t kernel_setupTaskStack(kTask_t startupPointer, kStackSize_t taskStackSize, kTaskType_t taskType);
void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler);

#endif /* MEMORY_H_ */