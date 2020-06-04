/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel.h>
#include "listutils.h"

static volatile uint8_t kTickRate = 0;
static volatile uint16_t kTaskActiveTicks = 0;

extern volatile uint16_t _kflags;

struct kLinkedListStruct_t* taskmgr_getReadyTaskListArray();

void taskmgr_initScheduler(kTaskHandle_t idle)
{

}

static inline void taskmgr_assign(kTaskHandle_t task)
{
	taskmgr_setNextTask(task);
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
}

//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void taskmgr_tickTasks()
{
	struct kLinkedListStruct_t* sleepingList = taskmgr_getSleepingTaskListPtr();
	kTaskHandle_t temp = sleepingList->head;
	
	while (temp != NULL) {
		if (temp->sleepTime) {
			temp->sleepTime--;
		}
		else {
			taskmgr_setTaskState(temp, KSTATE_READY);
		}
		temp = temp->taskList.next;
	}
}

static inline void taskmgr_search()
{
	struct kLinkedListStruct_t* priorityQueues = taskmgr_getReadyTaskListArray();
	for (int16_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		if (priorityQueues[i].head != NULL) {
			taskmgr_assign(priorityQueues[i].head);
			break;
		}
	}
}

void taskmgr_schedule()
{
	if (!kTickRate) {
		taskmgr_tickTasks();
		
		if (kTaskActiveTicks) {
			kTaskActiveTicks--;
		}
		else {
			if (utils_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED)) {
				taskmgr_search();
			}
		}
		
		kTickRate = CFG_TICKRATE_MS;
	}
	else {
		kTickRate--;
	}
	return;
}