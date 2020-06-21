/*
 * heap.h
 *
 * Created: 21.06.2020 20:09:34
 *  Author: Admin
 */


#ifndef HEAP_H_
#define HEAP_H_

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	uint8_t state;
};

#endif /* HEAP_H_ */