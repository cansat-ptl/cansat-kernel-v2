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
		//debug_puts(L_INFO, PSTR("\x0C"));
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

uint8_t kernel_startScheduler()
{
	//debug_puts(L_INFO, PSTR(" kernel: Starting up task manager                      [OK]\r\n"));
	//.................................................................

	//#if CFG_LOGGING == 1
	//	debug_puts(L_INFO, PSTR("kernel: Preparing safety memory barrier"));
	//#endif

	//kernel_prepareMemoryBarrier(kernel_getStackPtr() + (CFG_TASK_STACK_SIZE + CFG_KERNEL_STACK_SAFETY_MARGIN)-1, CFG_KERNEL_STACK_SAFETY_MARGIN, 0xFE);

	//#if CFG_LOGGING == 1
	//	debug_puts(L_NONE, PSTR("               [OK]\r\n"));
	//#endif

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

	debug_puts(L_INFO, PSTR("\x0C"));

	platform_ENABLE_INTERRUPTS();
	threads_exitCriticalSection();
	
	taskmgr_switchTo(taskmgr_getNextTaskHandle());

	return 0;
}

void kernel_startup()
{
	kernel_preinit();
	debug_puts(L_INFO, PSTR("initd: Pre-init phase\r\n"));
	user_preinit();

	debug_puts(L_INFO, PSTR("initd: Init phase\r\n"));
	user_init();

	debug_puts(L_INFO, PSTR("initd: Post-init phase\r\n"));
	user_postinit();

	debug_puts(L_INFO, PSTR("initd: Init complete, starting scheduler\r\n"));
	kernel_setSystemStatus(KOSSTATUS_RUNNING);
	kernel_startScheduler();
}
