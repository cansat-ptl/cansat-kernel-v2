/*
 * tasks.h
 *
 * Created: 21.06.2020 19:54:45
 *  Author: Admin
 */


#ifndef TASKS_INTERNAL_H_
#define TASKS_INTERNAL_H_

#define KPRIO_IDLE 0
#define KPRIO_REALTIME 255

#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kernel/tasks.h>
#include <stdint.h>
#include "../utils/linkedlists.h"

struct kNotificationStruct_t
{
	kEventState_t state;
	uint16_t eventFlags;
};

struct kTaskStruct_t
{
	kStackPtr_t stackPtr;
	kStackPtr_t stackBegin;
	kStackSize_t stackSize;
	
	kTask_t taskPtr;
	void* taskArgs;
	
	kTaskState_t state;
	kTaskTicks_t sleepTime;
	kTaskType_t type;
	uint8_t priority;
	uint8_t flags;
	kPid_t pid;
	char* name;
	
	volatile void* activeLock;
	volatile struct kNotificationStruct_t notification;
	
	volatile struct kListItemStruct_t activeTaskListItem;
	volatile struct kListItemStruct_t globalTaskListItem;
	
//	uint8_t savedContext[CFG_REGISTER_RESERVED_SPACE];
};

kReturnValue_t tasks_createTaskStatic(kStackPtr_t memory, kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t tasks_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);

kTaskHandle_t tasks_getNextTaskHandle();

volatile struct kLinkedListStruct_t* tasks_getReadyTaskListPtr(uint8_t priority);
volatile struct kLinkedListStruct_t* tasks_getSleepingTaskListPtr();

kTaskHandle_t tasks_getIdleTaskHandle();
kStackPtr_t tasks_getReservedMemoryPointer();
void tasks_setCurrentTask(kTaskHandle_t taskHandle);
void tasks_setNextTask(kTaskHandle_t taskHandle);

#endif /* TASKS_H_ */