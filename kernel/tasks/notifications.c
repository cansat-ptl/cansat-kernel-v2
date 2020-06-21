/*
 * event.c
 *
 * Created: 20.02.2020 22:08:22
 *  Author: Admin
 */

#include "notifications.h"
#include "tasks.h"
#include <kernel/ktypes.h>
#include <stdint.h>

kSpinlock_t notificationOpLock;

uint16_t tasks_notificationWait()
{
	uint16_t returnValue = 0;
	while (1) {
		threads_spinlockAcquire(&notificationOpLock);
		kTaskHandle_t runningTask = tasks_getCurrentTaskHandle();

		if (runningTask->notification.state == KEVENT_FIRED) {
			runningTask->notification.state = KEVENT_NONE;
			returnValue = runningTask->notification.eventFlags;
			threads_spinlockRelease(&notificationOpLock);
			break;
		}
		else {
			tasks_setTaskState(runningTask, KSTATE_SUSPENDED);
			threads_spinlockRelease(&notificationOpLock);
			tasks_sleep(0);
		}
	}
	return returnValue;
}

kReturnValue_t tasks_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags)
{
	kReturnValue_t exitcode = ERR_GENERIC;

	if (taskToNotify != NULL) {
		threads_spinlockAcquire(&notificationOpLock);

		if (taskToNotify->state == KSTATE_SUSPENDED) {
			tasks_setTaskState(taskToNotify, KSTATE_READY);
		}

		taskToNotify->notification.state = KEVENT_FIRED;
		taskToNotify->notification.eventFlags = flags;

		exitcode = 0;
		threads_spinlockRelease(&notificationOpLock);
	}
	else {
		exitcode = ERR_NULLPTR;
	}
	return exitcode;
}