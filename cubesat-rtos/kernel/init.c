/*
 * init.c
 *
 * Created: 13.02.2020 22:38:32
 *  Author: Admin
 */ 
#include <kernel.h>

void kernel_idle()
{
	while(1) hal_NOP();
}

uint8_t kernel_init()
{
	hal_UART_INIT(12);
	
	debug_puts(L_NONE, PSTR("[init] kernel: Startup\r\n")); //TODO: re-implement logging switch
	
	//debug_puts(L_NONE, PSTR("[init] kernel: Starting up task manager                      [OK]\r\n"));
	//debug_puts(L_NONE, PSTR("[init] kernel: Setting up idle task"));
	
	kernel_prepareMemoryBarrier(kernel_getStackPtr() + (TASK_STACK_SIZE + KERNEL_STACK_SAFETY_MARGIN)-1, KERNEL_STACK_SAFETY_MARGIN, 0xFE);
	
	kTaskHandle_t ct = kernel_createTask(kernel_idle, 64, KPRIO_NONE, KTASK_SYSTEM);
	if (ct == NULL) {
		//debug_puts(L_NONE, PSTR("                          [ERR]\r\n"));
		//debug_puts(L_NONE, PSTR("[init] kernel: Memory allocation error\r\n"));
		while(1);
	}
	ct -> pid = 0;
	//debug_puts(L_NONE, PSTR("                          [OK]\r\n"));
	
	//debug_puts(L_NONE, PSTR("===========================System=Info===========================\r\n"));
	//for (int i = 0; i < kTaskIndex; i++) {
	//	debug_puts(L_NONE, PSTR("-----------------------------------------------------------------\r\n"));
	//	debug_logMessage(PGM_ON, L_NONE, PSTR(" Task %2d: stackPtr:                                       0x%04X\r\n"), i, kTaskList[i].stackPtr);
	//	debug_logMessage(PGM_ON, L_NONE, PSTR("          taskPtr:                                        0x%04X\r\n"), kTaskList[i].taskPtr);
	//	debug_logMessage(PGM_ON, L_NONE, PSTR("          stackBegin:                                     0x%04X\r\n"), kTaskList[i].stackBegin);
	//	debug_logMessage(PGM_ON, L_NONE, PSTR("          stackSize:                                  %4d bytes\r\n"), kTaskList[i].stackSize);
	//	debug_logMessage(PGM_ON, L_NONE, PSTR("          priority:                                         %4d\r\n"), kTaskList[i].priority);
	//	debug_logMessage(PGM_ON, L_NONE, PSTR("          pid:                                              %4d\r\n"), kTaskList[i].pid);
	//}
	//debug_puts(L_NONE, PSTR("-----------------------------------------------------------------\r\n"));
	//debug_logMessage(PGM_ON, L_NONE, PSTR("Stack usage:                                           %4d bytes \r\n"), kUserTaskStackUsage + kSystemStackUsage);
	//debug_logMessage(PGM_ON, L_NONE, PSTR("Stack size:                                            %4d bytes \r\n"), TASK_STACK_SIZE + KERNEL_STACK_SIZE);
	//debug_puts(L_NONE, PSTR("=================================================================\r\n"));
	
	//debug_puts(L_NONE, PSTR("[init] kernel: Starting up first task"));
	kTaskHandle_t currentTask = kernel_getCurrentTaskHandle();
	currentTask = ct;
	//debug_puts(L_NONE, PSTR("                        [OK]\r\n"));
	
	//debug_puts(L_NONE, PSTR("[init] kernel: Setting up system timer"));
	hal_setupTimer0(3);
	//debug_puts(L_NONE, PSTR("                       [OK]\r\n"));
	
	//debug_puts(L_NONE, PSTR("[init] kernel: Starting up system timer"));
	hal_startTimer0();
	//debug_puts(L_NONE, PSTR("                      [OK]\r\n"));
	
	//debug_puts(L_NONE, PSTR("[init] kernel: System startup complete\r\n"));
	//_delay_ms(3000);
	
	//debug_puts(L_NONE, PSTR("\x0C"));
	
	hal_ENABLE_INTERRUPTS();
	kernel_exitCriticalSection();

	return 0;
}