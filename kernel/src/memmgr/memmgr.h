/*
 * memmgr.h
 *
 * Created: 16.06.2020 16:14:23
 *  Author: Admin
 */ 


#ifndef MEMMGR_H_
#define MEMMGR_H_

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	uint8_t state;
};

#endif /* MEMMGR_H_ */