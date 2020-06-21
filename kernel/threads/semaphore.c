/*
 * semaphore.c
 *
 * Created: 18.02.2020 1:10:19
 *  Author: Admin
 */

#include <kernel/threads/threads.h>
#include "../utils/linkedlists.h"

volatile struct kListItemStruct_t* tasks_getTaskListItem(kTaskHandle_t task); //TODO: add to header
void tasks_setTaskLock(kTaskHandle_t task, volatile struct kLockStruct_t* lock);
uint8_t tasks_getTaskPriority(kTaskHandle_t task);

kSpinlock_t semaphoreOpLock = 0;

struct kLockStruct_t threads_semaphoreInit(uint8_t resourceAmount)
{
	kSemaphore_t semaphore;
	semaphore.type = KLOCK_SEMAPHORE;
	semaphore.lockCount = resourceAmount;
	return semaphore;
}

static inline void threads_blockTask(volatile struct kLockStruct_t* lock, kTaskHandle_t task)
{
	if (task != NULL && lock != NULL) {
		volatile struct kListItemStruct_t* temp = tasks_getTaskListItem(task);
		if (temp->list != &lock->blockedTasks) {
			utils_listDeleteAny(temp->list, temp);
			utils_listAddBack(&(lock->blockedTasks), temp);
			tasks_setTaskState(task, KSTATE_BLOCKED);
			tasks_setTaskLock(task, lock);
		}
	}
	return;
}

static void threads_unblockTask(kTaskHandle_t task)
{
	if (task != NULL) {
		tasks_setTaskState(task, KSTATE_READY);
		tasks_setTaskLock(task, NULL);
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
					semaphore->lockOwner = tasks_getCurrentTaskHandle();
					semaphore->basePriority = tasks_getTaskPriority(semaphore->lockOwner);
				}

				exitcode = 0;
				threads_spinlockRelease(&semaphoreOpLock);
				break;
			}
			else {
				kTaskHandle_t currentTask = tasks_getCurrentTaskHandle();

				if (semaphore->type == KLOCK_MUTEX) {
					if (tasks_getTaskPriority(semaphore->lockOwner) < tasks_getTaskPriority(currentTask)) {
						tasks_setTaskPriority(semaphore->lockOwner, currentTask->priority);
					}
				}

				threads_blockTask(semaphore, currentTask);
				threads_spinlockRelease(&semaphoreOpLock);
				tasks_sleep(0);
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

		volatile struct kListItemStruct_t* temp = semaphore->blockedTasks.head;

		if (semaphore->type == KLOCK_MUTEX) {
			if (tasks_getTaskPriority(semaphore->lockOwner) != semaphore->basePriority) {
				tasks_setTaskPriority(semaphore->lockOwner, semaphore->basePriority);
				semaphore->lockOwner = NULL;
				semaphore->basePriority = 0;
			}
		}

		while(temp != NULL) {
			threads_unblockTask((kTaskHandle_t)temp->data);
			temp = temp->next;
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
