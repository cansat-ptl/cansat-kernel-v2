/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

void taskmgr_listAddBack(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task);
void taskmgr_listDeleteAny(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task);

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount)  //TODO: this function is a stub. You can help by improving it
{
	kSemaphore_t semaphore;
	semaphore.type = KLOCK_SEMAPHORE;
	semaphore.lockCount = resourceAmount;
	return semaphore;
}

static inline void threads_blockTask(volatile struct kLockStruct_t* lock, kTaskHandle_t task)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	if (task != NULL && lock != NULL) {
		if (task->taskList.list != &lock->blockedTasks) {
			taskmgr_listDeleteAny(task->taskList.list, task);
			taskmgr_listAddBack(&(lock->blockedTasks), task);
			task->state = KSTATE_BLOCKED;
			task->lock = lock;
		}
	}
	threads_endAtomicOperation(sreg);
	return;
}

static void threads_unblockTask(kTaskHandle_t task)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	if (task != NULL) {
		taskmgr_setTaskState(task, KSTATE_READY);
		task->lock = NULL;
	}
	
	threads_endAtomicOperation(sreg);
	return;
}

uint8_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		while (1) {
			kStatusRegister_t sreg = threads_startAtomicOperation();
		
			if (semaphore->lockCount != 0) {
				semaphore->lockCount--;			
				
				exitcode = 0;
				threads_endAtomicOperation(sreg);
				break;
			}
			else {
				threads_blockTask(semaphore, taskmgr_getCurrentTaskHandle());
				threads_endAtomicOperation(sreg);
				taskmgr_yield(0);
			}
		}
	}
	return exitcode;
}

uint8_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore)
{
	uint8_t exitcode = 1;
	if (semaphore != NULL) {
		//debug_logMessage(PGM_PUTS, L_NONE, PSTR("a"));
		kStatusRegister_t sreg = threads_startAtomicOperation();
		
		kTaskHandle_t temp = semaphore->blockedTasks.head;

		while(temp != NULL) {
			//debug_logMessage(PGM_ON, L_NONE, PSTR(""));
			threads_unblockTask(temp);
			temp = temp->taskList.next;
		}
		
		semaphore->lockCount++;
		
		exitcode = 0;
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}
