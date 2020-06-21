/*
 * taskutils.c
 *
 * Created: 19.06.2020 21:39:50
 *  Author: Admin
 */

#include <kernel/kernel.h>

struct kLockStruct_t;
struct kListItemStruct_t;

volatile struct kLockStruct_t* taskmgr_getTaskLock(kTaskHandle_t task)
{
	return task->lock;
}

void taskmgr_setTaskLock(kTaskHandle_t task, volatile struct kLockStruct_t* lock)
{
	task->lock = lock;
}

volatile struct kListItemStruct_t* taskmgr_getTaskListItem(kTaskHandle_t task)
{
	return task->itemPointer;
}

void taskmgr_setTaskListItem(kTaskHandle_t task, volatile struct kListItemStruct_t* itemPointer)
{
	task->itemPointer = itemPointer;
}

uint8_t taskmgr_getTaskPriority(kTaskHandle_t task)
{
	return task->priority;
}

kTaskState_t taskmgr_getTaskState(kTaskHandle_t task)
{
	return task->state;
}