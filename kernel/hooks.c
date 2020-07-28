/*
 * hooks.c
 *
 * Created: 05.05.2020 23:13:02
 *  Author: Admin
 */ 

#include <kernel/kernel_config.h>
#include <kernel/ktypes.h>
#include <kernel/kdefs.h>
#include <kdebug/debug.h>
#include <kernel/kernel.h>
#include "tasks/tasks.h"

void kernel_panic(const char * message) {
	kernel_setSystemStatus(KOSSTATUS_ERRORED);
	#if CFG_LOGGING == 1
		debug_puts(L_FATAL, PSTR("kernel: PANIC - "));
		debug_puts(L_NONE, message);
		
		kTaskHandle_t runningTask = tasks_getCurrentTaskHandle();
		
		debug_puts(L_FATAL, PSTR("kernel: Last executed task info:\r\n"));
		debug_logMessage(PGM_ON, L_FATAL, PSTR("    PID: %d; Name: %s; Entry: 0x%08X; Stack: 0x%08X; State: %d; Priority: %d;\r\n"), runningTask->pid, runningTask->name, runningTask->taskPtr, runningTask->stackPtr, runningTask->state, runningTask->priority);
		
		debug_puts(L_FATAL, PSTR("kernel: Memory info:\r\n"));
		debug_logMessage(PGM_ON, L_FATAL, PSTR("    HeapSize: %d; Free: %d; FreeWorst: %d; ProtectionMargin: %d; Reserved: %d;\r\n"), CFG_HEAP_SIZE, memory_getFreeHeap(), memory_getFreeHeapMin(), CFG_STACK_SAFETY_MARGIN, CFG_KERNEL_RESERVED_MEMORY);
	#endif
	
	#if CFG_KERNEL_PANIC_ACTION == 0
		#if CFG_LOGGING == 1
			debug_puts(L_FATAL, PSTR("kernel: Rebooting\r\n"));
		#endif
		hal_REBOOT();
	#endif
	
	#if CFG_KERNEL_PANIC_ACTION == 1
		kernel_setSystemStatus(KOSSTATUS_HALTED);
		#if CFG_LOGGING == 1
			debug_puts(L_FATAL, PSTR("kernel: System halted\r\n"));
		#endif
		while (1) { //TODO: reboot
			;//Do nothing
		}
	#endif
}

void kernel_taskReturnHook()
{	
	kTaskHandle_t runningTask = tasks_getCurrentTaskHandle();
	#if CFG_LOGGING == 1
		debug_puts(L_WARN, PSTR("kernel: Task return detected.\r\n"));
		debug_puts(L_WARN, PSTR("kernel: Returning task will be terminated.\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("kernel: Task info:\r\n"));
		debug_logMessage(PGM_ON, L_WARN, PSTR("PID: %d; Name: %s; Entry: 0x%08X; Stack: 0x%08X; State: %d; Priority: %d;\r\n"), runningTask->pid, runningTask->name, runningTask->taskPtr, runningTask->stackPtr, runningTask->state, runningTask->priority);
	#endif
	tasks_removeTask(runningTask);
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
		debug_logMessage(PGM_ON, L_WARN, PSTR("PID: %d; Name: %s; Entry: 0x%08X; Stack: 0x%08X; State: %d; Priority: %d;\r\n"), task->pid, task->name, task->taskPtr, task->stackPtr, task->state, task->priority);
	#endif
	tasks_removeTask(task);
	if (task->type == KTASK_SYSTEM) {
		kernel_panic(PSTR("System-critical task stack corruption\r\n"));
	}
}