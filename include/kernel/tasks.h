/*
 * taskmgr.h
 *
 * Created: 14.02.2020 20:50:36
 *  Author: Admin
 */ 


#ifndef TASKMGR_H_
#define TASKMGR_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

#define KPRIO_IDLE 0
#define KPRIO_REALTIME 255

kTaskHandle_t kernel_createTask(kTask_t t_pointer, kStackSize_t t_stackSize, uint8_t t_priority, kTaskType_t t_type);
uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state);
void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount);

kTaskHandle_t kernel_getCurrentTaskHandle();
kTaskHandle_t kernel_getNextTaskHandle();
kTaskHandle_t kernel_getTaskListPtr();
uint8_t kernel_getTaskListIndex();
void kernel_setCurrentTask(kTaskHandle_t taskHandle);

void kernel_yield(uint16_t sleep) __attribute__ (( naked, noinline ));
void kernel_stopTask(kTaskState_t exitState);
void kernel_switchTo(kTaskHandle_t handle) __attribute__ (( naked, noinline ));

#endif /* TASKMGR_H_ */