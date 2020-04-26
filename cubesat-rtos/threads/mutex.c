/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include <threads/threads.h>

struct kLock_t threads_mutexInit()  //TODO: this function is a stub. You can help by improving it
{
	struct kLock_t mutex;
	mutex.type = KLOCK_MUTEX;
	mutex.lockCount = 0;
	return mutex;
}


uint8_t threads_mutexLock(struct kLock_t* mutex)
{
	uint8_t exitcode = 1;
	if (mutex != NULL) {
		while (1) {
			uint8_t sreg = threads_startAtomicOperation();
			kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
			//debug_puts(L_INFO, PSTR("threads: attempting to lock mutex..."));
		
			if (mutex -> lockCount == 0 || mutex -> owner == NULL || mutex -> owner -> state == KSTATE_UNINIT) {
				mutex -> lockCount = 1;
				mutex -> owner = runningTask;
				//debug_puts(L_INFO, PSTR("success!\r\n"));
				exitcode = 0;
				threads_endAtomicOperation(sreg);
				break;
			}
			else {
				runningTask -> lock = mutex;
				//debug_puts(L_INFO, PSTR("error: locked\r\n"));
				kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_BLOCKED);
				threads_endAtomicOperation(sreg);
				kernel_yield(0);
			}
		}
	}
	return exitcode;
}

uint8_t threads_mutexUnlock(struct kLock_t* mutex)
{
	uint8_t exitcode = 1;
	
	//debug_puts(L_INFO, PSTR("threads: unlocking mutex\r\n"));
	if (mutex != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
		
		mutex -> lockCount = 0;
		mutex -> owner = NULL;
	
		kTaskHandle_t taskList = kernel_getTaskListPtr();
		uint8_t taskIndex = kernel_getTaskListIndex();

		for (int i = 0; i < taskIndex; i++) {
			if (taskList[i].lock == mutex) {
				//debug_puts(L_INFO, PSTR("threads: unlocking waiting tasks\r\n"));
				taskList[i].lock = NULL;
				kernel_setTaskState(&taskList[i], KSTATE_READY);
			}
		}
		exitcode = 0;
		threads_endAtomicOperation(sreg);
	}
	
	return exitcode;
}