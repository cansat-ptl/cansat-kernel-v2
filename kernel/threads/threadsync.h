/*
 * synchronisation.h
 *
 * Created: 21.06.2020 19:56:03
 *  Author: Admin
 */

#ifndef SYNCHRONISATION_INTERNAL_H_
#define SYNCHRONISATION_INTERNAL_H_

#include <stdint.h>
#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/threads.h>
#include "../utils/linkedlists.h"

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

#define THREADS_LOCK_STRUCT_SIZE (sizeof(struct kLockStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK)

#endif /* SYNCHRONISATION_H_ */