/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel.h>

static volatile uint8_t kTickRate = 0;
static volatile uint16_t kTaskActiveTicks = 0;

extern volatile uint16_t _kflags;

static struct kLinkedListStruct_t kPriorityQueue[CFG_NUMBER_OF_PRIORITIES];

void taskmgr_initScheduler(kTaskHandle_t idle)
{

}

static void taskmgr_addTaskToSchedulingList(kTaskHandle_t task)
{

}

void taskmgr_scheduleTask(kTaskHandle_t task)
{

}

static void taskmgr_removeTaskFromSchedulingList(uint8_t priority)
{

}

void taskmgr_unscheduleTask(kTaskHandle_t task)
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

}

static inline void taskmgr_search()
{
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