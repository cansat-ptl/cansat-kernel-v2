/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: ThePetrovich
 */ 

#include "../kernel.h"

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
			if (taskList[j].priority > taskList[j+1].priority) {
				swapFlag = 1;
				kernel_swapTasks(&taskList[j], &taskList[j+1]); //Bruh
			}
		}
		if (!swapFlag) break;
	}
	return;
}