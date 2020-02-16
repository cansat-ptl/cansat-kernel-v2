/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include <kernel.h>

struct kLock_t kernel_createMutex()  //TODO: this function is a stub. You can help by improving it
{
	struct kLock_t mutex;
	mutex.type = KLOCK_MUTEX;
	mutex.lockCount = 0;
	return mutex;
}


uint8_t kernel_lockMutex(struct kLock_t* mutex)
{
	if (mutex == NULL) return 1;
	
	while (1) {
		uint8_t sreg = kernel_startAtomicOperation();
		kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
		//debug_puts(L_NONE, PSTR("threads: attempting to lock mutex..."));
		
		if (mutex -> lockCount == 0 || mutex -> owner == NULL) {
			mutex -> lockCount = 1;
			mutex -> owner = runningTask;
			//debug_puts(L_NONE, PSTR("success!\r\n"));
			kernel_endAtomicOperation(sreg);
			return 0;
		}
		else {
			runningTask -> lock = mutex;
			//debug_puts(L_NONE, PSTR("error: locked\r\n"));
			kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_BLOCKED);
			kernel_endAtomicOperation(sreg);
			kernel_yield(0);
		}
	}
}

uint8_t kernel_unlockMutex(struct kLock_t* mutex)
{
	uint8_t sreg = kernel_startAtomicOperation();
	
	//debug_puts(L_NONE, PSTR("threads: unlocking mutex\r\n"));
	if (mutex == NULL) return 1;
		
	mutex -> lockCount = 0;
	mutex -> owner = NULL;
	
	kTaskHandle_t taskList = kernel_getTaskListPtr();
	uint8_t taskIndex = kernel_getTaskListIndex();

	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i].lock == mutex) {
			//debug_puts(L_NONE, PSTR("threads: unlocking waiting tasks\r\n"));
			taskList[i].lock = NULL;
			kernel_setTaskState(&taskList[i], KSTATE_READY);
		}
	}
	
	kernel_endAtomicOperation(sreg);
	return 0;
}