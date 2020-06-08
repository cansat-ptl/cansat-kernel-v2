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
#include <kernel/types.h>
#include <kernel/hal/hal.h>

#define KPRIO_IDLE 0
#define KPRIO_REALTIME 255

kReturnValue_t taskmgr_init(kTask_t idle);

kReturnValue_t taskmgr_createTaskStatic(kTaskHandle_t taskStruct, kStackPtr_t stack, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t taskmgr_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kTaskHandle_t taskmgr_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);

kReturnValue_t taskmgr_removeTask(kTaskHandle_t handle);

kReturnValue_t taskmgr_setTaskPriority(kTaskHandle_t task, uint8_t priority);
void taskmgr_setTaskState(kTaskHandle_t t_handle, kTaskState_t t_state);

kTaskHandle_t taskmgr_getCurrentTaskHandle();
kTaskHandle_t taskmgr_getNextTaskHandle();

volatile struct kLinkedListStruct_t* taskmgr_getReadyTaskListPtr(uint8_t priority);
volatile struct kLinkedListStruct_t* taskmgr_getSleepingTaskListPtr();

kTaskHandle_t taskmgr_getIdleTaskHandle();
kStackPtr_t taskmgr_getReservedMemoryPointer();
void taskmgr_setCurrentTask(kTaskHandle_t taskHandle);
void taskmgr_setNextTask(kTaskHandle_t taskHandle);

void taskmgr_sleep(kTaskTicks_t sleep);

void taskmgr_stopTask(kTaskState_t exitState);

#endif /* TASKMGR_H_ */