/*
 * tasks.h
 *
 * Created: 21.06.2020 20:13:55
 *  Author: Admin
 */


#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>

kTaskHandle_t tasks_getCurrentTaskHandle();

/* Tasks operations */
kTaskHandle_t tasks_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t tasks_removeTask(kTaskHandle_t task);
void tasks_restartTask(kTaskHandle_t task);
kTaskHandle_t tasks_forkTask(kTaskHandle_t task);
kReturnValue_t tasks_replaceTask(kTaskHandle_t taskToReplace, kTask_t entry, void* args);

void tasks_sleep(kTaskTicks_t sleep);

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);
kTaskState_t tasks_getTaskState(kTaskHandle_t task);

void tasks_setTaskType(kTaskHandle_t task, kTaskType_t newType);
kTaskType_t tasks_getTaskType(kTaskHandle_t task);

kStackSize_t tasks_getTaskStackSize(kTaskHandle_t task);

uint8_t tasks_getTaskPriority(kTaskHandle_t task);
kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, uint8_t priority);

uint16_t tasks_notificationWait();
kReturnValue_t tasks_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

#endif /* TASKS_H_ */