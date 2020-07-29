/*
 * taskutils.h
 *
 * Created: 21.06.2020 19:55:10
 *  Author: Admin
 */


#ifndef TASKUTILS_INTERNAL_H_
#define TASKUTILS_INTERNAL_H_

#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <stdint.h>

kStackPtr_t tasks_getTaskStackPointer(kTaskHandle_t task);
void tasks_setTaskStackPointer(kTaskHandle_t task, kStackPtr_t pointer);

kStackPtr_t tasks_getTaskStackBegin(kTaskHandle_t task);
void tasks_setTaskStackBegin(kTaskHandle_t task, kStackPtr_t pointer);

uint8_t tasks_checkStackBounds(kTaskHandle_t task);

#endif /* TASKUTILS_H_ */