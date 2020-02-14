/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */ 

#include <kernel.h>

void kernel_resetTask(kTaskHandle_t taskList, uint8_t position)
{
	taskList[position].stackPtr = NULL;
	taskList[position].stackSize = 0;
	taskList[position].priority = 0;
	taskList[position].taskPtr = NULL;
	taskList[position].stackBegin = NULL;
	taskList[position].lock = NULL;
	taskList[position].state = KSTATE_UNINIT;
	taskList[position].type = KTASK_SYSTEM;
	taskList[position].pid = 0;
}

void kernel_updateTaskPositions(kTaskHandle_t taskList, uint8_t amount)
{
	for (int i = 0; i < amount; i++) {
		taskList[i].position = i;
	}
}

inline void kernel_swapTasks(volatile struct kTaskStruct_t * taskA, volatile struct kTaskStruct_t * taskB)
{
	struct kTaskStruct_t taskTemp = *taskA;
	*taskA = *taskB;
	*taskB = taskTemp;
}

void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount) //Bubble sort
{
	uint8_t swapFlag = 0;
	
	for (int i = 0; i < amount; i++) {
		swapFlag = 0;
		for (int j = 0; j < amount-i; j++) {
			if (taskList[j].priority < taskList[j+1].priority) {
				swapFlag = 1;
				kernel_swapTasks(&taskList[j], &taskList[j+1]); //Bruh
			}
		}
		if (!swapFlag) break;
	}
	
	kernel_updateTaskPositions(taskList, amount);
	return;
}