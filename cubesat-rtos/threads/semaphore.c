/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */ 

#include <kernel.h>

struct kLock_t kernel_semaphoreInit(uint8_t resourceAmount)  //TODO: this function is a stub. You can help by improving it
{
	kSemaphore_t semaphore;
	semaphore.type = KLOCK_SEMAPHORE;
	semaphore.lockCount = resourceAmount;
	return semaphore;
}


uint8_t kernel_semaphoreWait(struct kLock_t* semaphore)
{
	if (semaphore == NULL) return 1;
	
	while (1) {
		uint8_t sreg = kernel_startAtomicOperation();
		kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
		//debug_puts(L_NONE, PSTR("threads: attempting to lock acquire semaphore..."));
		
		if (semaphore -> lockCount != 0) {
			semaphore -> lockCount--;
			//debug_puts(L_NONE, PSTR("success!\r\n"));
			kernel_endAtomicOperation(sreg);
			return 0;
		}
		else {
			runningTask -> lock = semaphore;
			//debug_puts(L_NONE, PSTR("error: occupied\r\n"));
			kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SEMAPHORE);
			kernel_endAtomicOperation(sreg);
			kernel_yield(0);
		}
	}
}

uint8_t kernel_semaphoreSignal(struct kLock_t* semaphore)
{
	uint8_t sreg = kernel_startAtomicOperation();
	
	//debug_puts(L_NONE, PSTR("threads: signaling semaphore\r\n"));
	if (semaphore == NULL) return 1;
	
	semaphore -> lockCount++;
	
	kTaskHandle_t taskList = kernel_getTaskListPtr();
	uint8_t taskIndex = kernel_getTaskListIndex();

	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i].lock == semaphore) {
			//debug_puts(L_NONE, PSTR("threads: unlocking waiting tasks\r\n"));
			taskList[i].lock = NULL;
			kernel_setTaskState(&taskList[i], KSTATE_READY);
		}
	}
	
	kernel_endAtomicOperation(sreg);
	return 0;
}
