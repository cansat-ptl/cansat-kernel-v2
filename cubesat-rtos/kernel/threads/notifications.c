/*
 * event.c
 *
 * Created: 20.02.2020 22:08:22
 *  Author: Admin
 */ 

#include <kernel/threads/threads.h>

void threads_notificationWait()
{
	while (1) {
		kStatusRegister_t sreg = threads_startAtomicOperation();
		kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
		if (runningTask -> notification.state == KEVENT_FIRED) {
			runningTask -> notification.state = KEVENT_NONE;
			threads_endAtomicOperation(sreg);
			break;
		}
		else {
			runningTask -> state = KSTATE_BLOCKED;
			threads_endAtomicOperation(sreg);
			kernel_yield(0);
		}
	}
	return;
}

uint8_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags)
{
	uint8_t exitcode = 1;
	if (taskToNotify != NULL) {
		kStatusRegister_t sreg = threads_startAtomicOperation();
	
		//debug_puts(L_INFO, PSTR("threads: unlocking mutex\r\n"));
	
		taskToNotify -> state = KSTATE_READY;
		taskToNotify -> notification.state = KEVENT_FIRED;
		taskToNotify -> notification.eventFlags = flags;
		
		exitcode = 0;
		threads_endAtomicOperation(sreg);
	}
	return exitcode;
}