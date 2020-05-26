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

extern volatile uint16_t kTaskActiveTicks;

static volatile kTaskHandle_t temp = NULL;

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
	temp = taskmgr_getTaskListPtr();
	while(temp != NULL) {
		if (temp->state == KSTATE_READY) {
			if (temp->priority > kCurrentTask->priority) {
				kNextTask = temp;
				break;
			}
			else if (temp->priority == kCurrentTask->priority) {
				if (kNextTaskIndex > kCurrentTaskIndex) {
					kNextTask = temp;
					break;
				}
			}
			else {
				kNextTask = temp;
				break;
			}
			kNextTaskIndex++;
		}
		else if (temp->state == KSTATE_SLEEPING) {
			if (temp->sleepTime) temp->sleepTime--;
			else temp->state = KSTATE_READY;
		}
		temp = temp->next;
	}
	taskmgr_assign();
	return;
}

void taskmgr_schedule()
{
	taskmgr_search();
	return;
}