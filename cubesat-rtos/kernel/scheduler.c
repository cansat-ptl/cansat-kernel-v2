/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */ 

#include <kernel.h>

static volatile kTaskHandle_t kSchedulingList;
static volatile uint8_t kSchedulingAmount;
static volatile uint8_t kCurrentTaskIdx = 0;
static volatile uint8_t kNextTaskIdx = 0;

extern volatile uint16_t kTaskActiveTicks;

void kernel_initScheduler(kTaskHandle_t taskList, uint8_t taskIndex)
{
	kSchedulingList = taskList;
	kSchedulingAmount = taskIndex;
	kCurrentTaskIdx = taskIndex; // Idle task should always be the last
}

static inline void scheduler_assign()
{
	kernel_setNextTask(&kSchedulingList[kNextTaskIdx]);
	kCurrentTaskIdx = kNextTaskIdx;
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
	kNextTaskIdx = 0;
	return;
}

static inline void scheduler_search()
{
	for (int i = 0; i < kSchedulingAmount+1; i++) {
		if (kSchedulingList[i].state == KSTATE_READY) {
			if (kSchedulingList[i].priority > kSchedulingList[kCurrentTaskIdx].priority) {
				kNextTaskIdx = i;
				scheduler_assign();
				return;
			}
			else if (kSchedulingList[i].priority == kSchedulingList[kCurrentTaskIdx].priority) {
				if (i > kCurrentTaskIdx) {
					kNextTaskIdx = i;
					scheduler_assign();
					return;
				}
			}
			else {
				kNextTaskIdx = i;
				scheduler_assign();
				return;
			}
		}
		else if (kSchedulingList[i].state == KSTATE_SLEEPING) {
			if (kSchedulingList[i].sleepTime) kSchedulingList[i].sleepTime--;
			else kSchedulingList[i].state = KSTATE_READY;
		}
	}
}

void kernel_schedule() 
{
	scheduler_search();
	return;
}