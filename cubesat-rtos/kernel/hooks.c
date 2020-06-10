/*
 * hooks.c
 *
 * Created: 05.05.2020 23:13:02
 *  Author: Admin
 */ 

#include <kernel/kernel.h>

void kernel_panic(const char * message) {
	#if CFG_LOGGING == 1
		debug_puts(L_FATAL, PSTR("kernel: PANIC - "));
		debug_puts(L_NONE, message);
	#endif
	while (1) { //TODO: reboot
		;//Do nothing
	}
}

void kernel_taskReturnHook()
{
	kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
	#if CFG_LOGGING == 1
		debug_puts(L_WARN, PSTR("kernel: Task return detected.\r\n"));
		debug_puts(L_WARN, PSTR("kernel: Returning task will be terminated.\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("kernel: Task info:\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("PID: %d; Name: %s; Entry: 0x%08X; StackBegin: 0x%08X;\r\n"), runningTask->pid, runningTask->name, runningTask->taskPtr, runningTask->stackBegin);
	#endif
	taskmgr_removeTask(runningTask);
	while (1) {
		;//Do nothing
	}
}

void kernel_stackCorruptionHook(kTaskHandle_t task)
{
	#if CFG_LOGGING == 1
		debug_puts(L_ERROR, PSTR("kernel: Task memory corruption detected.\r\n"));
		debug_puts(L_ERROR, PSTR("kernel: Corrupted task will be terminated.\r\n"));
		debug_logMessage(PGM_ON, L_ERROR, PSTR("kernel: Task info:\r\n"));
		debug_logMessage(PGM_ON, L_ERROR, PSTR("PID: %d; Name: %s; Entry: 0x%08X; StackBegin: 0x%08X;\r\n"), task->pid, task->name, task->taskPtr, task->stackBegin);
	#endif
	taskmgr_removeTask(task);
	if (task->type == KTASK_SYSTEM) {
		kernel_panic(PSTR("System-critical task stack corruption\r\n"));
	}
}