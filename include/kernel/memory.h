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
#include <kernel/types.h>
#include <kernel/hal/hal.h>

kStackPtr_t kernel_getStackPtr();
kStackSize_t kernel_getUserTaskStackUsage();
kStackSize_t kernel_getSystemTaskStackUsage();

void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler);
uint8_t kernel_checkStackProtectionRegion(kTaskHandle_t checkedTask);

void kernel_heapInit();
void* kernel_heapAlloc(size_t size);
void kernel_heapFree(void* pointer);

size_t kernel_getFreeHeap();
size_t kernel_getFreeHeapMin();

#endif /* MEMORY_H_ */