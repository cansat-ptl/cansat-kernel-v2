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

static volatile uint8_t kSchedState = 0;
static volatile uint8_t kSwitchReady = 0;

extern volatile uint16_t kTaskActiveTicks;

void kernel_initScheduler(kTaskHandle_t taskList, uint8_t taskIndex)
{
	kSchedulingList = taskList;
	kSchedulingAmount = taskIndex;
	kCurrentTaskIdx = taskIndex;
}

static inline void scheduler_stateSearching()
{
	for (int i = 0; i < kSchedulingAmount+1; i++) {
		if (kSchedulingList[i].state == KSTATE_READY) {
			if (kSchedulingList[i].priority > kSchedulingList[kCurrentTaskIdx].priority) {
				kNextTaskIdx = i;
				kSchedState = 2;
				return;
			}
			else if (kSchedulingList[i].priority == kSchedulingList[kCurrentTaskIdx].priority) {
				if (i > kCurrentTaskIdx) {
					kNextTaskIdx = i;
					kSchedState = 2;
					return;
				}
			}
			else {
				kNextTaskIdx = i;
				kSchedState = 2;
				return;
			}
		}
		else if (kSchedulingList[i].state == KSTATE_SLEEPING) {
			if (kSchedulingList[i].sleepTime) kSchedulingList[i].sleepTime--;
			else kSchedulingList[i].state = KSTATE_READY;
		}
	}
}

static inline void scheduler_stateFound() 
{
	kernel_setNextTask(&kSchedulingList[kNextTaskIdx]);
	kCurrentTaskIdx = kNextTaskIdx;
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
	kNextTaskIdx = 0;
	kSwitchReady = 1;
	return;
}

static inline void scheduler_stateFoundLower()
{
	kSchedState = 0;
	kNextTaskIdx = 0;
	kCurrentTaskIdx = 0;
	return;
}

void kernel_schedule() 
{
	while (!kSwitchReady) {
		if (kSchedState == 0) scheduler_stateSearching();
		else if (kSchedState == 1) scheduler_stateFoundLower();
		else if (kSchedState == 2) scheduler_stateFound();
		else return;
	}
	kSwitchReady = 0;
	kSchedState = 0;
	return;
}