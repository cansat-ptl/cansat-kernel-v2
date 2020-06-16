/*
 * tasks.h
 *
 * Created: 16.06.2020 16:29:28
 *  Author: Admin
 */ 


#ifndef TASKS_H_
#define TASKS_H_

struct kTaskStruct;
typedef volatile struct kTaskStruct_t* kTaskHandle_t;

kReturnValue_t taskmgr_init(kTask_t idle);

kTaskHandle_t taskmgr_createTask(kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name);
kReturnValue_t taskmgr_removeTask(kTaskHandle_t task);
kTaskHandle_t taskmgr_forkTask(kTaskHandle_t task);
kReturnValue_t taskmgr_replaceTask(kTaskHandle_t taskToReplace, kTask_t entry, void* args);
void taskmgr_restartTask(kTaskHandle_t task);

kReturnValue_t taskmgr_setTaskPriority(kTaskHandle_t task, uint8_t priority);
void taskmgr_setTaskState(kTaskHandle_t handle, kTaskState_t newState);

kTaskHandle_t taskmgr_getCurrentTaskHandle();
kTaskHandle_t taskmgr_getNextTaskHandle();

void taskmgr_sleep(kTaskTicks_t sleep);

#endif /* TASKS_H_ */