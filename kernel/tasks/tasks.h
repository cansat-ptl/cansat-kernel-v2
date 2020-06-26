/*
 * tasks.h
 *
 * Created: 21.06.2020 19:54:45
 *  Author: Admin
 */


#ifndef TASKS_H_
#define TASKS_H_

#define KPRIO_IDLE 0
#define KPRIO_REALTIME 255

#include <kernel/tasks.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <stdint.h>

struct kNotificationStruct_t
{
	kEventState_t state;
	uint16_t eventFlags;
};

struct kTaskStruct_t
{
	kStackPtr_t stackPtr;
	kTask_t taskPtr;
	void* args;
	kStackPtr_t stackBegin;
	kStackSize_t stackSize;
	kTaskState_t state;
	kTaskTicks_t sleepTime;
	uint8_t priority;
	uint8_t flags;
	kTaskType_t type;
	kPid_t pid;
	char* name;
	volatile struct kLockStruct_t* lock;
	volatile struct kNotificationStruct_t notification;
	volatile struct kListItemStruct_t taskListItem;
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