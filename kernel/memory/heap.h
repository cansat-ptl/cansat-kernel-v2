/*
 * heap.h
 *
 * Created: 21.06.2020 20:09:34
 *  Author: Admin
 */


#ifndef HEAP_INTERNAL_H_
#define HEAP_INTERNAL_H_

#include <kernel/ktypes.h>
#include <stdint.h>

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	uint8_t state;
};

void memory_heapInit();
kReturnValue_t memory_pointerSanityCheck(void* pointer);

void memory_prepareProtectionRegion(void* pointer, int16_t size);
uint8_t memory_checkProtectionRegion(void* pointer, int16_t size);

#endif /* HEAP_H_ */