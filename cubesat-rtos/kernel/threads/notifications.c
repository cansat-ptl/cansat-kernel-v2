/*
 * event.c
 *
 * Created: 20.02.2020 22:08:22
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

kSpinlock_t notificationOpLock;

uint16_t threads_notificationWait()
{
	uint16_t returnValue = 0;
	while (1) {
		threads_spinlockAcquire(&notificationOpLock);
		kTaskHandle_t runningTask = taskmgr_getCurrentTaskHandle();
		
		if (runningTask->notification.state == KEVENT_FIRED) {
			runningTask->notification.state = KEVENT_NONE;
			returnValue = runningTask->notification.eventFlags;
			threads_spinlockRelease(&notificationOpLock);
			break;
		}
		else {
			taskmgr_setTaskState(runningTask, KSTATE_SUSPENDED);
			threads_spinlockRelease(&notificationOpLock);
			taskmgr_sleep(0);
		}
	}
	return returnValue;
}

uint8_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags)
{
	uint8_t exitcode = 1;
	if (taskToNotify != NULL) {
		threads_spinlockAcquire(&notificationOpLock);
	
		if (taskToNotify->state == KSTATE_SUSPENDED) {
			taskmgr_setTaskState(taskToNotify, KSTATE_READY);			
		}
		
		taskToNotify->notification.state = KEVENT_FIRED;
		taskToNotify->notification.eventFlags = flags;
		
		exitcode = 0;
		threads_spinlockRelease(&notificationOpLock);
	}
	return exitcode;
}