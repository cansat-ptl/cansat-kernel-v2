/*
 * semaphores.h
 *
 * Created: 16.06.2020 16:36:29
 *  Author: Admin
 */ 


#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

#endif /* SEMAPHORES_H_ */