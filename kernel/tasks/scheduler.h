/*
 * scheduler.h
 *
 * Created: 21.06.2020 19:54:57
 *  Author: Admin
 */


#ifndef SCHEDULER_INTERNAL_H_
#define SCHEDULER_INTERNAL_H_

#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <stdint.h>

void tasks_initScheduler(kTaskHandle_t idle);
void tasks_setActiveTicks(uint16_t activeTicks);
void tasks_schedule();

#endif /* SCHEDULER_H_ */