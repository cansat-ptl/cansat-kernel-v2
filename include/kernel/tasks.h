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

kTaskHandle_t kernel_createTask(kTask_t startupPointer, void* args, kStackSize_t taskStackSize, uint8_t taskPriority, kTaskType_t taskType, char* name);
uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state);
void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount);
uint8_t kernel_removeTask(kTaskHandle_t handle);

kTaskHandle_t kernel_getCurrentTaskHandle();
kTaskHandle_t kernel_getNextTaskHandle();
kTaskHandle_t kernel_getTaskListPtr();
uint8_t kernel_getTaskListIndex();
void kernel_setCurrentTask(kTaskHandle_t taskHandle);
void kernel_setNextTask(kTaskHandle_t taskHandle);

void kernel_tick()  __attribute__ ( ( naked, noinline ));
void kernel_yield(uint16_t sleep) __attribute__ (( naked, noinline ));
void kernel_switchTo(kTaskHandle_t handle) __attribute__ (( naked, noinline ));
void kernel_stopTask(kTaskState_t exitState);

#endif /* TASKMGR_H_ */