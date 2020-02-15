/*
 * init.c
 *
 * Created: 13.02.2020 22:38:32
 *  Author: Admin
 */ 
#include <kernel.h>

void kernel_idle()
{
	while(1) platform_NOP();
}

uint8_t kernel_init()
{
	hal_UART_INIT(12);
	
	debug_puts(L_NONE, PSTR("[init] kernel: Startup\r\n")); //TODO: re-implement logging switch
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager                      [OK]\r\n"));
	debug_puts(L_NONE, PSTR("[init] kernel: Setting up idle task"));
	
	kernel_prepareMemoryBarrier(kernel_getStackPtr() + (TASK_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN)-1, KERNEL_STACK_SAFETY_MARGIN, 0xFE);
	
	kTaskHandle_t ct = kernel_createTask(kernel_idle, 64, 0, KTASK_SYSTEM);
	if (ct == NULL) {
		debug_puts(L_NONE, PSTR("                          [ERR]\r\n"));
		debug_puts(L_NONE, PSTR("[init] kernel: Memory allocation error\r\n"));
		while(1);
	}
	ct -> pid = 0;
	debug_puts(L_NONE, PSTR("                          [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up first task"));
	kernel_setCurrentTask(ct);
	debug_puts(L_NONE, PSTR("                        [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Setting up system timer"));
	platform_setupTimer0(3);
	debug_puts(L_NONE, PSTR("                       [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: Starting up system timer"));
	platform_startTimer0();
	debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	
	debug_puts(L_NONE, PSTR("[init] kernel: System startup complete\r\n"));
	_delay_ms(100);
	
	debug_puts(L_NONE, PSTR("\x0C"));
	
	platform_ENABLE_INTERRUPTS();
	kernel_exitCriticalSection();

	return 0;
}