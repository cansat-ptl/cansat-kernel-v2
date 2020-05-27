/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel.h>

static volatile kTaskHandle_t kSchedulingList;
static volatile kTaskHandle_t kNextTask;
static volatile kTaskHandle_t kCurrentTask;
static volatile kTaskHandle_t kIdleTask;

static volatile uint8_t kCurrentTaskIndex;
static volatile uint8_t kNextTaskIndex;

static volatile uint8_t kTickRate = 0;
static volatile uint16_t kTaskActiveTicks = 0;

extern volatile uint16_t _kflags;

kTaskHandle_t taskmgr_getIdleTaskHandle()
{
	return kIdleTask;
}

void taskmgr_initScheduler(kTaskHandle_t idle)
{
	kIdleTask = idle;
	kCurrentTask = idle;
	kNextTask = idle;
}

static inline void taskmgr_assign()
{
	taskmgr_setNextTask(kNextTask);
	kCurrentTask = kNextTask;
	kCurrentTaskIndex = kNextTaskIndex;
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
	kNextTask = kIdleTask;
	kNextTaskIndex = 0;
	return;
}
//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void taskmgr_search()
{
	register kTaskHandle_t temp = taskmgr_getTaskListPtr();
	register uint8_t found = 0;
	while(temp != NULL) {
		if (temp->state == KSTATE_SLEEPING) {
			if (temp->sleepTime) temp->sleepTime--;
			else temp->state = KSTATE_READY;
		}
		
		if (hal_CHECK_BIT(_kflags, KFLAG_CSW_ALLOWED) && !found) {
			if (temp->state == KSTATE_READY) {
				if (temp->priority > kCurrentTask->priority) {
					kNextTask = temp;
					found = 1;
					continue;
				}
				else if (temp->priority == kCurrentTask->priority) {
					if (kNextTaskIndex > kCurrentTaskIndex) {
						kNextTask = temp;
						found = 1;
						continue;
					}
				}
				else {
					kNextTask = temp;
					found = 1;
					continue;
				}
				kNextTaskIndex++;
			}
		}
		temp = temp->next;
	}
	if (kTaskActiveTicks) kTaskActiveTicks--;
	if (found) taskmgr_assign();
	return;
}

void taskmgr_schedule()
{
	if (!kTickRate) {
		taskmgr_search();
		kTickRate = CFG_TICKRATE_MS;
	}
	else {
		kTickRate--;
	}
	return;
}