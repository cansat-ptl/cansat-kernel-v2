/*
 * init.c
 *
 * Created: 13.02.2020 22:38:32
 *  Author: Admin
 */ 
#include <kernel.h>

extern volatile kTaskHandle_t kIdleTaskHandle;

void kernel_initScheduler(kTaskHandle_t taskList, uint8_t taskIndex);

void kernel_idle()
{
	while(1) platform_NOP();
}

void kernel_init()
{
	hal_UART_INIT(12);
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("[init] kernel: Initializing debug uart interface, baud=38400\r\n"));
	#endif
}

uint8_t kernel_startScheduler()
{
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager"));
	#endif
	
	kernel_initScheduler(kernel_getTaskListPtr(), kernel_getTaskListIndex());
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	#endif
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("[init] kernel: Preparing safety memory barrier"));
	#endif
	
	kernel_prepareMemoryBarrier(kernel_getStackPtr() + (CFG_TASK_STACK_SIZE + CFG_KERNEL_STACK_SAFETY_MARGIN)-1, CFG_KERNEL_STACK_SAFETY_MARGIN, 0xFE);
	
	#if CFG_LOGGING == 1
		debug_puts(L_NONE, PSTR("               [OK]\r\n"));
	#endif
	
	kTaskHandle_t ct = kernel_createTask(kernel_idle, 64, 0, KTASK_SYSTEM);
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