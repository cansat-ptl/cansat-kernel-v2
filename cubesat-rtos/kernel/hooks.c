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
	/*debug_logMessage(PGM_ON, L_FATAL, PSTR("kernel: Task return detected\r\n"));
	debug_logMessage(PGM_ON, L_FATAL, PSTR("kernel: Executing task return hook\r\n"));

	kStatusRegister_t sreg = threads_startAtomicOperation();

	kTaskHandle_t handle = taskmgr_getCurrentTaskHandle();
	kTaskHandle_t taskList = taskmgr_getTaskListPtr();
	uint8_t taskIndex = taskmgr_getTaskListIndex();

	handle -> state = KSTATE_UNINIT;
	
	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i] -> lock -> owner == handle) {
			taskList[i] -> state = KSTATE_READY;
			taskList[i] -> lock -> owner = NULL;
			taskList[i] -> lock -> lockCount = 0;
		}
	}

	threads_endAtomicOperation(sreg);
	taskmgr_yield(0);

	while(1);*/
}

void kernel_stackCorruptionHook()
{
	/*kStatusRegister_t sreg = threads_startAtomicOperation();

	kTaskHandle_t handle = taskmgr_getCurrentTaskHandle();
	kTaskHandle_t taskList = taskmgr_getTaskListPtr();
	uint8_t taskIndex = taskmgr_getTaskListIndex();

	handle -> state = KSTATE_UNINIT;
	
	debug_puts(L_INFO, PSTR("kernel: Executing task corruption hook\r\n")); //TODO: debug information
	
	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i] -> lock -> owner == handle) {
			taskList[i] -> state = KSTATE_READY;
			taskList[i] -> lock -> owner = NULL;
			taskList[i] -> lock -> lockCount = 0;
		}
	}
	
	threads_endAtomicOperation(sreg);
	taskmgr_yield(0);
	while(1);*/
}