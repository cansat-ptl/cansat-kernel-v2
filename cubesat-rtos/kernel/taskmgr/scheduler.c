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

static struct kSchedulerQueueStruct_t kPriorityQueue[CFG_NUMBER_OF_PRIORITIES];

void taskmgr_initScheduler(kTaskHandle_t idle)
{
	for (uint16_t i = 0; i < CFG_NUMBER_OF_PRIORITIES; i++) {
		kPriorityQueue[i].head = NULL;
		kPriorityQueue[i].tail = NULL;
	}
	kPriorityQueue[KPRIO_IDLE].head = idle;
	kPriorityQueue[KPRIO_IDLE].tail = idle;
}

static void taskmgr_addTaskToSchedulingList(kTaskHandle_t task)
{
	task->schedulingList.next = NULL;
	task->schedulingList.prev = kPriorityQueue[task->priority].tail;
	
	if (kPriorityQueue[task->priority].tail) {
		kPriorityQueue[task->priority].tail->schedulingList.next = task;
	}
	
	kPriorityQueue[task->priority].tail = task;
	
	if (kPriorityQueue[task->priority].head == NULL) {
		kPriorityQueue[task->priority].head = task;
	}
}

void taskmgr_scheduleTask(kTaskHandle_t task)
{
	taskmgr_addTaskToSchedulingList(task);
}

static void taskmgr_removeTaskFromSchedulingList(uint8_t priority)
{
	kTaskHandle_t prev;

	if (kPriorityQueue[priority].head != NULL) {
		prev = kPriorityQueue[priority].head;
		kPriorityQueue[priority].head = kPriorityQueue[priority].head->schedulingList.next;
		
		if (kPriorityQueue[priority].head) {
			kPriorityQueue[priority].head->schedulingList.prev = NULL;
		}
		
		if (prev == kPriorityQueue[priority].tail) {
			kPriorityQueue[priority].tail = NULL;
		}
	}
}

void taskmgr_unscheduleTask(kTaskHandle_t task)
{
	if (task != NULL) {
		if (kPriorityQueue[task->priority].head == task) {
			kPriorityQueue[task->priority].head = task->schedulingList.next;
		}
		
		if (task->schedulingList.next != NULL) {
			task->schedulingList.next->schedulingList.prev = task->schedulingList.prev;
		}

		if (task->schedulingList.prev != NULL) {
			task->schedulingList.prev->schedulingList.next = task->schedulingList.next;
		}
	}
}

static inline void taskmgr_assign(kTaskHandle_t task)
{
	taskmgr_setNextTask(task);
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
}

//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void taskmgr_tickTasks()
{
	register kTaskHandle_t temp = taskmgr_getTaskListPtr();
	
	while(temp != NULL) {
		if (temp->state == KSTATE_SLEEPING) {
			if (temp->sleepTime) {
				temp->sleepTime--;
			}
			else {
				temp->state = KSTATE_READY;
			}
		}
		if (temp->state == KSTATE_READY) {
			taskmgr_addTaskToSchedulingList(temp);
			temp->state = KSTATE_RUNNING;
		}
		temp = temp->taskList.next;
	}
	
	return;
}

static inline void taskmgr_search()
{
	for (uint16_t i = CFG_NUMBER_OF_PRIORITIES-1; i > 0; i--) {
		if (kPriorityQueue[i].head != NULL) {
			taskmgr_assign(kPriorityQueue[i].head);
			if (kPriorityQueue[i].head->state == KSTATE_RUNNING) kPriorityQueue[i].head->state = KSTATE_READY;
			taskmgr_removeTaskFromSchedulingList(kPriorityQueue[i].head->priority);
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