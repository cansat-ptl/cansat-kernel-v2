/*
 * mutex.c
 *
 * Created: 12.02.2020 22:00:08
 *  Author: Admin
 */ 

#include "../kernel.h"
#include "../types.h"

struct kLock_t kernel_createMutex()  //TODO: this function is a stub. You can help by improving it
{
	struct kLock_t mutex;
	mutex.type = KLOCK_MUTEX;
	mutex.locks = 0;
	return mutex;
}

uint8_t kernel_lockMutex(struct kLock_t* mutex)
{
	kernel_enterCriticalSection();
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	
	if (mutex == NULL) return 1;
	
	kTaskHandle_t runningTask = kernel_getCurrentTaskHandle();
	runningTask -> lock = mutex;
	
	if (mutex -> locks == 0) {
		mutex -> locks = 1;
		hal_ENABLE_INTERRUPTS();
		hal_STATUS_REG = sreg;
		kernel_exitCriticalSection();
		return 0;
	}
	else {
		kernel_setTaskState(runningTask, KSTATE_BLOCKED);
		hal_ENABLE_INTERRUPTS();
		hal_STATUS_REG = sreg;
		kernel_exitCriticalSection();
		kernel_yield();
		return 1;
	}
}

uint8_t kernel_unlockMutex(struct kLock_t* mutex)
{
	kernel_enterCriticalSection();
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	if (mutex == NULL) return 1;
	
	mutex -> locks = 0;
	kTaskHandle_t taskList = kernel_getTaskListPtr();
	uint8_t taskIndex = kernel_getTaskListIndex();
	
	for (int i = 0; i < taskIndex; i++) {
		if (taskList[i].lock == mutex) {
			taskList[i].lock = NULL;
			kernel_setTaskState(&taskList[i], KSTATE_READY);
		}
	}
	
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
	kernel_exitCriticalSection();
	return 0;
}