/*
 * tasks.h
 *
 * Created: 16.06.2020 16:34:51
 *  Author: Admin
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include <tasks.h>
#include "../platform/ptypes.h"

typedef void kTask;
typedef void (*kTask_t)(void*);

typedef volatile struct kTaskStruct_t* kTaskHandle_t;
	
struct kLinkedListStruct_t
{
	kTaskHandle_t head;
	kTaskHandle_t tail;
};

struct kListItemStruct_t
{
	volatile struct kLinkedListStruct_t* list;
	kTaskHandle_t next;
	kTaskHandle_t prev;
};

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
	volatile struct kLockStruct_t* lock;
	volatile struct kNotificationStruct_t notification;
	kTaskState_t state;
	kTaskTicks_t sleepTime;
	kTaskType_t type;
	uint8_t priority;
	uint8_t flags;
	kPid_t pid;
	char* name;
	struct kListItemStruct_t taskList;
};

kReturnValue_t taskmgr_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t taskmgr_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);

volatile struct kLinkedListStruct_t* taskmgr_getReadyTaskListPtr(uint8_t priority);
volatile struct kLinkedListStruct_t* taskmgr_getSleepingTaskListPtr();

kReturnValue_t taskmgr_init(kTask_t idle);

kTaskHandle_t taskmgr_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t taskmgr_removeTask(kTaskHandle_t task);
kTaskHandle_t taskmgr_forkTask(kTaskHandle_t task);
kReturnValue_t taskmgr_replaceTask(kTaskHandle_t taskToReplace, kTask_t entry, void* args);
void taskmgr_restartTask(kTaskHandle_t task);

kReturnValue_t taskmgr_setTaskPriority(kTaskHandle_t task, uint8_t priority);
void taskmgr_setTaskState(kTaskHandle_t handle, kTaskState_t newState);

void taskmgr_sleep(kTaskTicks_t sleep);
#endif /* TASKS_H_ */