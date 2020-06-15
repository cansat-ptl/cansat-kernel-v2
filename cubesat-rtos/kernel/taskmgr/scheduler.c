/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel.h>
#include "listutils.h"

static volatile uint8_t kTickRate = 0;
static volatile kTaskTicks_t kTaskActiveTicks = 0;

extern volatile uint16_t _kflags;

struct kLinkedListStruct_t* taskmgr_getReadyTaskListArray();

void taskmgr_setActiveTicks(uint16_t activeTicks)
{
	kTaskActiveTicks = activeTicks;
}

void taskmgr_initScheduler(kTaskHandle_t idle)
{
	struct kLinkedListStruct_t* priorityQueues = taskmgr_getReadyTaskListArray();
	priorityQueues[KPRIO_IDLE].head = idle;
	priorityQueues[KPRIO_IDLE].tail = idle;
}

static inline void taskmgr_assign(kTaskHandle_t task)
{
	taskmgr_setNextTask(task);
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
}

//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void taskmgr_tickTasks()
{
	volatile struct kLinkedListStruct_t* sleepingList = taskmgr_getSleepingTaskListPtr();
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
	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		if (priorityQueues[i].head != NULL) {
			
			#if CFG_MEMORY_PROTECTION_MODE != 0
				if (memmgr_pointerSanityCheck((void*)priorityQueues[i].head) != 0) {
					kernel_panic(PSTR("Memory access violation in scheduler: priorityQueues.head is out of bounds\r\n"));
				}
			#endif
			
			taskmgr_assign(priorityQueues[i].head);
			kTaskHandle_t temp = priorityQueues[i].head;
			taskmgr_listDropFront(&priorityQueues[i]);
			taskmgr_listAddBack(&priorityQueues[i], temp);
			break;
		}
	}
}

void taskmgr_schedule()
{
	if (!kTickRate) {
		taskmgr_tickTasks();
		kTickRate = CFG_TICKRATE_MS;
		if (kTaskActiveTicks) {
			kTaskActiveTicks--;
		}
	}
	else {
		kTickRate--;
	}
	
	if (!kTaskActiveTicks) {
		if (utils_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED)) {
			taskmgr_search();
		}
	}
	return;
}