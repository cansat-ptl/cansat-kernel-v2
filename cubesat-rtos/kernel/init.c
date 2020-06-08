/*
 * init.c
 *
 * Created: 13.02.2020 22:38:32
 *  Author: Admin
 */
#include <kernel/kernel.h>

void user_preinit();
void user_init();
void user_postinit();

kTask kernel_idle1(void* args)
{
	platform_ENABLE_INTERRUPTS();
	threads_exitCriticalSection();
	while(1) {
		platform_NOP();
		//debug_logMessage(PGM_PUTS, L_INFO, PSTR("idle: idle task debug output\r\n"));
	}
}

void kernel_preinit()
{
	hal_UART_INIT(24);
	debug_init();
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("kernel: Initializing debug uart interface, baud=38400\r\n"));
		debug_puts(L_INFO, PSTR("kernel: Firing up RTOS\r\n"));
		debug_puts(L_INFO, PSTR("kernel: Initializing memory manager\r\n"));
	#endif	
	memmgr_heapInit();
	
	#if CFG_LOGGING == 1	
		debug_puts(L_INFO, PSTR("kernel: Initializing task manager\r\n"));
	#endif
	taskmgr_init(kernel_idle1);
}

kReturnValue_t kernel_startScheduler()
{
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("kernel: Setting up system timer"));
	#endif

	platform_setupSystemTimer();
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                       [OK]\r\n"));
		debug_puts(L_INFO, PSTR("kernel: Starting up system timer"));
	#endif

	platform_startSystemTimer();

	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
		debug_puts(L_INFO, PSTR("kernel: System startup complete\r\n"));
	#endif

	platform_DELAY_MS(1000);
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("\x0C"));
	#endif
	
	kernel_idle1(NULL);
	return 0;
}

void kernel_startup()
{
	kernel_preinit();
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("initd: Pre-init phase\r\n"));
	#endif
	user_preinit();
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("initd: Init phase\r\n"));
	#endif
	user_init();
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("initd: Post-init phase\r\n"));
	#endif
	user_postinit();
	
	#if CFG_LOGGING == 1
		debug_puts(L_INFO, PSTR("initd: Init complete, starting scheduler\r\n"));
	#endif
	
	kernel_setSystemStatus(KOSSTATUS_RUNNING);
	kernel_startScheduler();
}
