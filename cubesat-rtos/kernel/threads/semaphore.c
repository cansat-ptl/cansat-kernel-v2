/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */

#include <kernel/threads/threads.h>

kSpinlock_t semaphoreOpLock = 0;

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
	if (task != NULL && lock != NULL) {
		if (task->taskList.list != &lock->blockedTasks) {
			taskmgr_listDeleteAny(task->taskList.list, task);
			taskmgr_listAddBack(&(lock->blockedTasks), task);
			task->state = KSTATE_BLOCKED;
			task->lock = lock;
		}
	}
	return;
}

static void threads_unblockTask(kTaskHandle_t task)
{
	if (task != NULL) {
		taskmgr_setTaskState(task, KSTATE_READY);
		task->lock = NULL;
	}
	return;
}

kReturnValue_t threads_semaphoreWait(volatile struct kLockStruct_t* semaphore)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (semaphore != NULL) {
		while (1) {
			threads_spinlockAcquire(&semaphoreOpLock);

			if (semaphore->lockCount != 0) {
				semaphore->lockCount--;
				
				if (semaphore->type == KLOCK_MUTEX) {
					semaphore->lockOwner = taskmgr_getCurrentTaskHandle();
					semaphore->basePriority = semaphore->lockOwner->priority;
				}

				exitcode = 0;
				threads_spinlockRelease(&semaphoreOpLock);
				break;
			}
			else {
				kTaskHandle_t currentTask = taskmgr_getCurrentTaskHandle();
				
				if (semaphore->type == KLOCK_MUTEX) {
					if (semaphore->lockOwner->priority < currentTask->priority) {
						taskmgr_setTaskPriority(semaphore->lockOwner, currentTask->priority);
					}
				}
				
				threads_blockTask(semaphore, currentTask);
				threads_spinlockRelease(&semaphoreOpLock);
				taskmgr_sleep(0);
			}
		}
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}

kReturnValue_t threads_semaphoreSignal(volatile struct kLockStruct_t* semaphore)
{
	kReturnValue_t exitcode = ERR_GENERIC;
	if (semaphore != NULL) {
		threads_spinlockAcquire(&semaphoreOpLock);

		kTaskHandle_t temp = semaphore->blockedTasks.head;
		
		if (semaphore->type == KLOCK_MUTEX) {
			if (semaphore->lockOwner->priority != semaphore->basePriority) {
				taskmgr_setTaskPriority(semaphore->lockOwner, semaphore->basePriority);
				semaphore->lockOwner = NULL;
				semaphore->basePriority = 0;
			}
		}

		while(temp != NULL) {
			threads_unblockTask(temp);
			temp = temp->taskList.next;
		}

		semaphore->lockCount++;

		exitcode = 0;
		threads_spinlockRelease(&semaphoreOpLock);
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}
