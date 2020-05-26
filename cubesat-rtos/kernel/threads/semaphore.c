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
			kStatusRegister_t sreg = threads_startAtomicOperation();
			kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
		
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
				taskmgr_setTaskState(taskmgr_getCurrentTaskHandle(), KSTATE_SEMAPHORE);
				threads_endAtomicOperation(sreg);
				taskmgr_yield(0);
			}
		}
	}
	return exitcode;
}

uint8_t threads_semaphoreSignal(struct kLock_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		kStatusRegister_t sreg = threads_startAtomicOperation();
	
		//debug_puts(L_INFO, PSTR("threads: signaling semaphore\r\n"));
		semaphore -> lockCount++;
	
		kTaskHandle_t temp = taskmgr_getTaskListPtr();

		while(temp != NULL) {
			if (temp->lock == semaphore) {
				//debug_puts(L_INFO, PSTR("threads: unlocking waiting tasks\r\n"));
				temp->lock = NULL;
				taskmgr_setTaskState(temp, KSTATE_READY);
			}
			temp = temp->next;
		}
		
		exitcode = 0;
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}
