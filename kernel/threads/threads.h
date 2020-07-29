/*
 * threads.h
 *
 * Created: 29.07.2020 13:33:07
 *  Author: Admin
 */ 


#ifndef THREADS_INTERNAL_H_
#define THREADS_INTERNAL_H_

#include "../utils/linkedlists.h"
#include <kernel/threads.h>
#include <stdint.h>
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

#define THREADS_LOCK_STRUCT_SIZE (sizeof(struct kLockStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK)

#endif /* THREADS_H_ */