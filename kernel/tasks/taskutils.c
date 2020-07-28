/*
 * taskutils.c
 *
 * Created: 19.06.2020 21:39:50
 *  Author: Admin
 */

#include "taskutils.h"
#include "tasks.h"
#include <kernel/ktypes.h>
#include <stdint.h>

struct kLockStruct_t;
struct kListItemStruct_t;

//TODO: sanity checks & internal functions

volatile struct kLockStruct_t* tasks_getTaskLock(kTaskHandle_t task)
{
	return task->lock;
}

void tasks_setTaskLock(kTaskHandle_t task, volatile struct kLockStruct_t* lock)
{
	task->lock = lock;
}

volatile struct kListItemStruct_t* tasks_getTaskListItem(kTaskHandle_t task)
{
	return task->itemPointer;
}

void tasks_setTaskListItem(kTaskHandle_t task, volatile struct kListItemStruct_t* itemPointer)
{
	task->itemPointer = itemPointer;
}

uint8_t tasks_getTaskPriority(kTaskHandle_t task)
{
	return task->priority;
}

//void tasks_setTaskPriority(kTaskHandle_t task, uint8_t priority); //Declared in tasks.c

kTaskState_t tasks_getTaskState(kTaskHandle_t task)
{
	return task->state;
}

//void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state); //Declared in tasks.c

kTaskType_t tasks_getTaskType(kTaskHandle_t task)
{
	return task->type;
}

void tasks_setTaskType(kTaskHandle_t task, kTaskType_t newType)
{
	task->type = newType;
}

kStackPtr_t tasks_getTaskStackPointer(kTaskHandle_t task)
{
	return task->stackPtr;
}

void tasks_setTaskStackPointer(kTaskHandle_t task, kStackPtr_t pointer)
{
	task->stackPtr = pointer;
}

kStackPtr_t tasks_getTaskStackBegin(kTaskHandle_t task)
{
	return task->stackBegin;
}

void tasks_setTaskStackBegin(kTaskHandle_t task, kStackPtr_t pointer)
{
	task->stackBegin = pointer;
}

kStackSize_t tasks_getTaskStackSize(kTaskHandle_t task)
{
	return task->stackSize;
}

uint8_t tasks_checkStackBounds(kTaskHandle_t task) {
	uint8_t exitcode = 0;
	if (task->type != KTASK_SYSTEM) {
		if (task->stackPtr < task->stackBegin || task->stackBegin + task->stackSize < task->stackPtr) {
			exitcode = 1;
		}
	}
	return exitcode;
}