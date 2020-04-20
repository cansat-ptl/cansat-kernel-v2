/*
 * init.c
 *
 * Created: 13.02.2020 22:38:32
 *  Author: Admin
 */ 
#include <initd/initd.h>

extern volatile kTaskHandle_t kIdleTaskHandle;

void kernel_initScheduler(kTaskHandle_t taskList, uint8_t taskIndex);

void user_preinit();
void user_init();
void user_postinit();

void kernel_idle()
{
	while(1) platform_NOP();
}

void kernel_preinit()
{
	hal_UART_INIT(12);
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("\x0C"));
		debug_puts(L_NONE, PSTR("[init] kernel: Initializing debug uart interface, baud=38400\r\n"));
	#endif
}

uint8_t kernel_startScheduler()
{
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager"));
	#endif
	//debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager                      [OK]\r\n"));
	//.................................................................
	kernel_initScheduler(kernel_getTaskListPtr(), kernel_getTaskListIndex());
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: Preparing safety memory barrier"));
	#endif
	
	kernel_prepareMemoryBarrier(kernel_getStackPtr() + (CFG_TASK_STACK_SIZE + CFG_KERNEL_STACK_SAFETY_MARGIN)-1, CFG_KERNEL_STACK_SAFETY_MARGIN, 0xFE);
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("               [OK]\r\n"));
	#endif
	
	kTaskHandle_t ct = kernel_createTask(kernel_idle, NULL, 64, KPRIO_IDLE, KTASK_SYSTEM, "idle");
	if (ct == NULL) {
		while(1);
	}
	ct -> pid = 0;
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("[init] kernel: Starting up first task"));
	#endif
	
	kIdleTaskHandle = ct;
	kernel_setCurrentTask(ct);
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                        [OK]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: Setting up system timer"));
	#endif
	
	platform_setupSystemTimer();
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                       [OK]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: Starting up system timer"));
	#endif
	
	platform_startSystemTimer();
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: System startup complete\r\n"));
	#endif
	
	platform_DELAY_MS(1000);
	
	debug_puts(L_NONE, PSTR("\x0C"));
	
	platform_ENABLE_INTERRUPTS();
	threads_exitCriticalSection();

	return 0;
}

void initd_startup()
{
	kernel_preinit();
	debug_puts(L_NONE, PSTR("[init] initd: Pre-init phase\r\n"));
	user_preinit();
	
	debug_puts(L_NONE, PSTR("[init] initd: Init phase\r\n"));
	user_init();
	
	debug_puts(L_NONE, PSTR("[init] initd: Post-init phase\r\n"));
	user_postinit();
	
	debug_puts(L_NONE, PSTR("[init] initd: Init complete, starting scheduler\r\n"));
	kernel_startScheduler();
}
