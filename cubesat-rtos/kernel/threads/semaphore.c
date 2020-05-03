/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

struct kLock_t threads_semaphoreInit(uint8_t resourceAmount)  //TODO: this function is a stub. You can help by improving it
{
	kSemaphore_t semaphore;
	semaphore.type = KLOCK_SEMAPHORE;
	semaphore.lockCount = resourceAmount;
	return semaphore;
}


uint8_t threads_semaphoreWait(struct kLock_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		while (1) {
			uint8_t sreg = threads_startAtomicOperation();
			kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
			//debug_puts(L_INFO, PSTR("threads: attempting to lock acquire semaphore..."));
		
			if (semaphore -> lockCount != 0) {
				semaphore -> lockCount--;
				//debug_puts(L_INFO, PSTR("success!\r\n"));
				exitcode = 0;
				threads_endAtomicOperation(sreg);
				break;
			}
			else {
				runningTask -> lock = semaphore;
				//debug_puts(L_INFO, PSTR("error: occupied\r\n"));
				kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SEMAPHORE);
				threads_endAtomicOperation(sreg);
				kernel_yield(0);
			}
		}
	}
	return exitcode;
}

uint8_t threads_semaphoreSignal(struct kLock_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		uint8_t sreg = threads_startAtomicOperation();
	
		//debug_puts(L_INFO, PSTR("threads: signaling semaphore\r\n"));
		semaphore -> lockCount++;
	
		kTaskHandle_t taskList = kernel_getTaskListPtr();
		uint8_t taskIndex = kernel_getTaskListIndex();

		for (int i = 0; i < taskIndex; i++) {
			if (taskList[i].lock == semaphore) {
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
