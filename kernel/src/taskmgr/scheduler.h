/*
 * scheduler.h
 *
 * Created: 16.06.2020 16:34:36
 *  Author: Admin
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <tasks.h>

void taskmgr_initScheduler(kTaskHandle_t idle);

void taskmgr_schedule();

void taskmgr_setActiveTicks(uint16_t activeTicks);

#endif /* SCHEDULER_H_ */