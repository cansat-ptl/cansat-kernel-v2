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
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("systemd: Startup\r\n"));
		debug_puts(L_INFO, PSTR("systemd: Preparing service lists"));
	#endif

	systemd_clearServiceQueue();
	systemd_clearCallQueue();

	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
		debug_puts(L_INFO, PSTR("systemd: Setting up software timer"));
	#endif

	kernel_setTimer(systemd_tick, CFG_SYSTEMD_TIMER_TICKS);

	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                    [OK]\r\n"));
		debug_puts(L_INFO, PSTR("systemd: Creating host task\r\n"));
	#endif

	taskmgr_createTask(systemd_main, NULL, 256, CFG_SYSTEMD_TASK_PRIORITY, KTASK_SYSTEM, "systemd");

	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("systemd: Startup complete\r\n"));
	#endif
}