/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */ 

#include "config.h"
#include <math.h>
#include <avr/io.h>

uint8_t resourse = 1;

kMutex_t mutex0, mutex1;
kSemaphore_t semaphore0;
kSpinlock_t spinlock0;
kTaskHandle_t t1, t2, t3;

kTask simpleTask()
{
	kernel_yield(1500);
	while (1) {
		//kernel_acquireSpinlock(&spinlock0);
		debug_puts(L_NONE, PSTR("Job 1 starts, acquiring spinlock\r\n"));
		_delay_ms(50);
		debug_puts(L_NONE, PSTR("Job 1 ends, releasing spinlock\r\n"));
		//kernel_releaseSpinlock(&spinlock0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask1()
{
	kernel_yield(1500);
	while (1) {
		//kernel_acquireSpinlock(&spinlock0);
		debug_puts(L_NONE, PSTR("Job 2 starts, acquiring spinlock\r\n"));
		_delay_ms(50);
		debug_puts(L_NONE, PSTR("Job 2 ends, releasing spinlock\r\n"));
		//kernel_releaseSpinlock(&spinlock0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

kTask simpleTask2()
{
	kernel_yield(1500);
	while (1) {
		kernel_waitSemaphore(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 3 starts, waiting for semaphore\r\n"));
		resourse *= 3;
		kernel_yield(400);
		debug_puts(L_NONE, PSTR("Job 3 ends, signaling semaphore\r\n"));
		kernel_signalSemaphore(&semaphore0);
		kernel_yield(400);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask3()
{
	while (1) {
		kernel_waitSemaphore(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 4 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 4 ends, signaling semaphore\r\n"));
		kernel_signalSemaphore(&semaphore0);
		kernel_yield(200);
	}
}
kTask simpleTask4()
{
	while (1) {
		kernel_waitSemaphore(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 5 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 5 ends, signaling semaphore\r\n"));
		kernel_signalSemaphore(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask5()
{
	while (1) {
		kernel_waitSemaphore(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 6 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 6 ends, signaling semaphore\r\n"));
		kernel_signalSemaphore(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

int main()
{
	mutex0 = kernel_createMutex();
	semaphore0 = kernel_createSemaphore(2);
	
	kernel_createTask(simpleTask, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask1, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask2, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask3, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask4, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask5, 150, 3, KTASK_USER);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}