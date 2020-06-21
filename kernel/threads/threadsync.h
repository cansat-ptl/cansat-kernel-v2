/*
 * synchronisation.h
 *
 * Created: 21.06.2020 19:56:03
 *  Author: Admin
 */


#ifndef SYNCHRONISATION_H_
#define SYNCHRONISATION_H_

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

#endif /* SYNCHRONISATION_H_ */