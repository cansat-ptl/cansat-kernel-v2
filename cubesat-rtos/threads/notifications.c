/*
 * event.c
 *
 * Created: 20.02.2020 22:08:22
 *  Author: Admin
 */ 

#include <threads/threads.h>

uint8_t threads_notificationWait()
{
	while (1) {
		uint8_t sreg = threads_startAtomicOperation();
		kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
		
		if (runningTask -> notification.state == KEVENT_FIRED) {
			runningTask -> notification.state = KEVENT_NONE;
			threads_endAtomicOperation(sreg);
			return 0;
		}
		else {
			runningTask -> state = KSTATE_BLOCKED;
			threads_endAtomicOperation(sreg);
			kernel_yield(0);
		}
	}
}

uint8_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags)
{
	uint8_t sreg = threads_startAtomicOperation();
	
	//debug_puts(L_NONE, PSTR("threads: unlocking mutex\r\n"));
	if (taskToNotify == NULL) return 1;
	
	taskToNotify -> state = KSTATE_READY;
	taskToNotify -> notification.state = KEVENT_FIRED;
	taskToNotify -> notification.eventFlags = flags;
	
	threads_endAtomicOperation(sreg);
	return 0;
}