/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

struct kLock_t threads_mutexInit()
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
			kStatusRegister_t sreg = threads_startAtomicOperation();
			kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
		
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
				taskmgr_setTaskState(runningTask, KSTATE_BLOCKED);
				threads_endAtomicOperation(sreg);
				taskmgr_yield(0);
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
		kStatusRegister_t sreg = threads_startAtomicOperation();
		
		mutex -> lockCount = 0;
		mutex -> owner = NULL;
	
		kTaskHandle_t temp = taskmgr_getTaskListPtr();
		
		while(temp != NULL) {
			if (temp->lock == mutex) {
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