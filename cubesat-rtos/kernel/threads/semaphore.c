/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount)  //TODO: this function is a stub. You can help by improving it
{
	kSemaphore_t semaphore;
	semaphore.type = KLOCK_SEMAPHORE;
	semaphore.lockCount = resourceAmount;
	semaphore.owner = taskmgr_getIdleTaskHandle();
	return semaphore;
}


uint8_t threads_semaphoreWait(struct kLockStruct_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		while (1) {
			kStatusRegister_t sreg = threads_startAtomicOperation();
			kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
		
			if (semaphore->lockCount != 0 || semaphore->owner->state == KSTATE_UNINIT) {
				semaphore->lockCount--;
				
				if (semaphore->owner->priority <= runningTask->priority) semaphore->owner = runningTask;
				
				exitcode = 0;
				threads_endAtomicOperation(sreg);
				break;
			}
			else {
				runningTask -> lock = semaphore;
				taskmgr_setTaskState(runningTask, KSTATE_BLOCKED);
				threads_endAtomicOperation(sreg);
				taskmgr_yield(0);
			}
		}
	}
	return exitcode;
}

uint8_t threads_semaphoreSignal(struct kLockStruct_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		kStatusRegister_t sreg = threads_startAtomicOperation();
		kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
		kTaskHandle_t idle = taskmgr_getIdleTaskHandle();
	
		//debug_puts(L_INFO, PSTR("threads: signaling semaphore\r\n"));
		semaphore->lockCount++;
		
		runningTask->lock = NULL;
		semaphore->owner = idle;
	
		kTaskHandle_t temp = taskmgr_getTaskListPtr();

		while(temp != NULL) {
			if (temp->lock == semaphore) {
				if (temp->state == KSTATE_BLOCKED) taskmgr_setTaskState(temp, KSTATE_READY);
				if (temp->priority >= semaphore->owner->priority) semaphore->owner = temp;
			}
			temp = temp->taskList.next;
		}
		
		exitcode = 0;
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}
