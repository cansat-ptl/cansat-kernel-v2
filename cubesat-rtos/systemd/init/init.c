/*
 * init.c
 *
 * Created: 22.02.2020 21:24:12
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

void systemd_init()
{
	debug_puts(L_NONE, PSTR("[init] systemd: Startup\r\n"));
	debug_puts(L_NONE, PSTR("[init] systemd: Preparing service lists"));
	
	systemd_clearServiceQueue();
	systemd_clearCallQueue();
	
	debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	debug_puts(L_NONE, PSTR("[init] systemd: Setting up software timer"));
	
	kernel_setTimer(systemd_tick, CFG_SYSTEMD_TIMER_TICKS);
	
	debug_puts(L_NONE, PSTR("                    [OK]\r\n"));
	debug_puts(L_NONE, PSTR("[init] systemd: Creating host task\r\n"));
	
	kernel_createTask(systemd_main, 100, CFG_SYSTEMD_TASK_PRIORITY, KTASK_SYSTEM);
	
	debug_puts(L_NONE, PSTR("[init] systemd: Startup complete\r\n"));
}