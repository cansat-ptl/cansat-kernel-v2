/*
 * scheduler.c
 *
 * Created: 16.02.2020 18:09:53
 *  Author: Admin
 */

#include <kernel/kernel.h>

static volatile kTaskHandle_t* kSchedulingList;
static volatile uint8_t kSchedulingAmount;
static volatile uint8_t kCurrentTaskIdx = 0;
static volatile uint8_t kNextTaskIdx = 0;
static volatile uint8_t kIdleTaskIdx = 0;

static kTaskHandle_t kIdleTaskHandle;

extern volatile uint16_t kTaskActiveTicks;

static void taskmgr_setIdleTask(kTaskHandle_t idle)
{
	kIdleTaskHandle = idle;
}

void taskmgr_init(kTaskHandle_t* taskQueue, uint8_t taskIndex)
{
	kSchedulingList = taskQueue;
	kSchedulingAmount = taskIndex;
	kIdleTaskIdx = kSchedulingAmount;
	
	for (int i = 0; i < CFG_MAX_TASK_COUNT; i++) {
		if (kSchedulingList[i] -> pid == 0 && kSchedulingList[i] -> type == KTASK_SYSTEM) {
			kIdleTaskIdx = i;
			break;
		}
	}
	kCurrentTaskIdx = kSchedulingAmount; // Idle task should always be the last
}

static inline void taskmgr_assign()
{
	taskmgr_setNextTask(kSchedulingList[kNextTaskIdx]);
	kCurrentTaskIdx = kNextTaskIdx;
	kTaskActiveTicks = CFG_TICKS_PER_TASK;
	kNextTaskIdx = kIdleTaskIdx;
	return;
}

static inline void taskmgr_search()
{
	for (int i = 0; i < kSchedulingAmount+1; i++) {
		if (kSchedulingList[i] -> state == KSTATE_READY) {
			if (kSchedulingList[i] -> priority > kSchedulingList[kCurrentTaskIdx] -> priority) {
				kNextTaskIdx = i;
			}
			else if (kSchedulingList[i] -> priority == kSchedulingList[kCurrentTaskIdx] -> priority) {
				if (i > kCurrentTaskIdx) {
					kNextTaskIdx = i;
				}
			}
			else {
				kNextTaskIdx = i;
			}
		}
		else if (kSchedulingList[i] -> state == KSTATE_SLEEPING) {
			if (kSchedulingList[i] -> sleepTime) kSchedulingList[i] -> sleepTime--;
			else kSchedulingList[i] -> state = KSTATE_READY;
		}
	}
	taskmgr_assign();
	return;
}

void taskmgr_schedule()
{
	taskmgr_search();
	return;
}