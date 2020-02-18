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

kLifo_t queue0;

kTask simpleTask()
{
	kernel_yield(1500);
	debug_puts(L_NONE, PSTR("Job 1 starts, creating queue\r\n"));
	
	char buff[32];
	char hello[] = "Hello from task1!";
	uint8_t hellosize = strlen(hello);
	queue0 = threads_lifoInit(buff, 32);
	
	while (1) {
		threads_mutexLock(&mutex0);
		debug_puts(L_NONE, PSTR("Job 1 runs, sending character to queue\r\n"));
		for (int i = hellosize+1; i >= 0; i--) {
			if (threads_lifoWrite(&queue0, hello[i])) {
				debug_logMessage(PGM_ON, L_NONE, PSTR("Lifo write success, written character = %c\r\n"), hello[i]);
			}
			else {
				debug_logMessage(PGM_PUTS, L_NONE, PSTR("Lifo write error\r\n"));
			}
		}
		debug_puts(L_NONE, PSTR("Job 1 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		kernel_yield(200);
	}
}
kTask simpleTask1()
{
	kernel_yield(1500);
	debug_puts(L_NONE, PSTR("Job 2 starts\r\n"));
	char hello1[32];
	while (1) {
		threads_mutexLock(&mutex0);
		uint8_t pos = 0;
		while (threads_lifoAvailable(&queue0)) {
			hello1[pos] = threads_lifoRead(&queue0);
			pos++;
		}
		debug_logMessage(PGM_ON, L_NONE, PSTR("Lifo contents: %s\r\n"), hello1);
		
		debug_puts(L_NONE, PSTR("Job 2 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		kernel_yield(200);
	}
}

kTask simpleTask2()
{
	kernel_yield(1500);
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 3 starts, waiting for semaphore\r\n"));
		resourse *= 3;
		kernel_yield(400);
		debug_puts(L_NONE, PSTR("Job 3 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(400);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask3()
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 4 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 4 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
	}
}
kTask simpleTask4()
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 5 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 5 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask5()
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_NONE, PSTR("Job 6 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 6 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

int main()
{
	mutex0 = threads_mutexInit();
	semaphore0 = threads_semaphoreInit(2);
	
	kernel_createTask(simpleTask, 250, 5, KTASK_USER);
	kernel_createTask(simpleTask1, 250, 5, KTASK_USER);
	//kernel_createTask(simpleTask2, 150, 3, KTASK_USER);
	//kernel_createTask(simpleTask3, 150, 2, KTASK_USER);
	//kernel_createTask(simpleTask4, 150, 1, KTASK_USER);
	//kernel_createTask(simpleTask5, 250, 3, KTASK_USER);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}