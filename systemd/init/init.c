/*
 * init.c
 *
 * Created: 22.02.2020 21:24:12
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

void systemd_init()
{
	systemd_clearServiceQueue();
	systemd_clearCallQueue();
	kernel_setTimer(systemd_tick, CFG_SYSTEMD_TIMER_TICKS);
	kernel_createTask(systemd_main, 100, CFG_SYSTEMD_TASK_PRIORITY, KTASK_SYSTEM);
}