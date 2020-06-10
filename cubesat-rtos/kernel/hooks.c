/*
 * hooks.c
 *
 * Created: 05.05.2020 23:13:02
 *  Author: Admin
 */ 

#include <kernel/kernel.h>

//TODO: Fix these

void kernel_taskReturnHook()
{
	kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
	#if CFG_LOGGING == 1
		debug_puts(L_WARN, PSTR("kernel: Task return detected.\r\n"));
		debug_puts(L_WARN, PSTR("kernel: Returning task will be terminated.\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("kernel: Task info:\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("kernel: PID: %d; Name: %s; Entry: 0x%08X; StackBegin: 0x%08X;\r\n"), runningTask->pid, runningTask->name, runningTask->taskPtr, runningTask->stackBegin);
	#endif
	taskmgr_removeTask(runningTask);
	while (1) {
		;//Do nothing
	}
}

void kernel_stackCorruptionHook()
{
	
}

void kernel_panic(const char * message) {
	#if CFG_LOGGING == 1
		debug_puts(L_FATAL, message);
	#endif
	while (1) { //TODO: reboot
		;//Do nothing
	}
}