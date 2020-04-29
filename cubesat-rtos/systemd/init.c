/*
 * init.c
 *
 * Created: 22.02.2020 21:24:12
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

void systemd_main();
void systemd_tick();

void systemd_init()
{
	debug_puts(L_INFO, PSTR("systemd: Startup\r\n"));
	debug_puts(L_INFO, PSTR("systemd: Preparing service lists"));
	
	systemd_clearServiceQueue();
	systemd_clearCallQueue();
	
	debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	debug_puts(L_INFO, PSTR("systemd: Setting up software timer"));
	
	kernel_setTimer(systemd_tick, CFG_SYSTEMD_TIMER_TICKS);
	
	debug_puts(L_NONE, PSTR("                    [OK]\r\n"));
	debug_puts(L_INFO, PSTR("systemd: Creating host task\r\n"));
	
	kernel_createTask(systemd_main, NULL, 100, CFG_SYSTEMD_TASK_PRIORITY, KTASK_SYSTEM, "systemd");
	
	debug_puts(L_INFO, PSTR("systemd: Startup complete\r\n"));
}