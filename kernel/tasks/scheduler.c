/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel_config.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include "scheduler.h"
#include "tasks.h"
#include "../memory/memory.h"
#include "../memory/heap.h"
#include "../utils/linkedlists.h"
#include "../utils/utils.h"

static volatile uint8_t kTickRate = 0;
static volatile kTaskTicks_t kTaskActiveTicks = 0;

extern volatile uint16_t _kflags;

struct kLinkedListStruct_t* tasks_getReadyTaskListArray();

void kernel_panic(const char * message);

void tasks_setActiveTicks(uint16_t activeTicks)
{
	kTaskActiveTicks = activeTicks;
}

void tasks_initScheduler(kTaskHandle_t idle)
{
	struct kLinkedListStruct_t* priorityQueues = tasks_getReadyTaskListArray();
	priorityQueues[KPRIO_IDLE].head = &(idle->activeTaskListItem);
	priorityQueues[KPRIO_IDLE].tail = &(idle->activeTaskListItem);
}

static inline void tasks_assign(volatile struct kListItemStruct_t* listItem)
{
	tasks_setNextTask((kTaskHandle_t)listItem->data);
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
}

//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void tasks_tickTasks()
{
	volatile struct kLinkedListStruct_t* sleepingList = tasks_getSleepingTaskListPtr();
	volatile struct kListItemStruct_t* temp = sleepingList->head;

	while (temp != NULL) {
		if (((kTaskHandle_t)(temp->data))->sleepTime) {
			((kTaskHandle_t)(temp->data))->sleepTime--;
		}
		else {
			tasks_setTaskState((kTaskHandle_t)temp->data, KSTATE_READY);
		}
		temp = temp->next;
	}
}

static inline void tasks_search()
{
	struct kLinkedListStruct_t* priorityQueues = tasks_getReadyTaskListArray();
	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		if (priorityQueues[i].head != NULL) {

			#if CFG_MEMORY_PROTECTION_MODE != 0
				if (memory_pointerSanityCheck((void*)priorityQueues[i].head) != 0) {
					kernel_panic(PSTR("Memory access violation in scheduler: priorityQueues.head is out of bounds\r\n"));
				}
			#endif

			tasks_assign(priorityQueues[i].head);
			volatile struct kListItemStruct_t* temp = priorityQueues[i].head;
			utils_listDropFront(&priorityQueues[i]);
			utils_listAddBack(&priorityQueues[i], temp);
			break;
		}
	}
}

void tasks_schedule()
{
	if (!kTickRate) {
		tasks_tickTasks();
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
			tasks_search();
		}
	}
	return;
}