/*
 * taskmgr.h
 *
 * Created: 16.06.2020 16:15:11
 *  Author: Admin
 */ 


#ifndef TASKMGR_H_
#define TASKMGR_H_

#include <stdint.h>
#include <tasks.h>
#include "../platform/ptypes.h"

kStackPtr_t taskmgr_getKernelStackPointer();
kStackPtr_t taskmgr_getReservedMemoryPointer();

void taskmgr_setKernelStackPointer(kStackPtr_t pointer);
void taskmgr_setCurrentTask(kTaskHandle_t taskHandle);
void taskmgr_setNextTask(kTaskHandle_t taskHandle);

#endif /* TASKMGR_H_ */