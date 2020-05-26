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

void memmgr_heapInit();
void* memmgr_heapAlloc(size_t size);
void memmgr_heapFree(void* pointer);

size_t memmgr_getFreeHeap();
size_t memmgr_getFreeHeapMin();

#endif /* MEMORY_H_ */