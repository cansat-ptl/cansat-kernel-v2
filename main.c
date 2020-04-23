/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */

#include "config.h"
#include <math.h>
#include <avr/io.h>
#include <systemd/systemd.h>
#include <initd/initd.h>

uint8_t resourse = 1;

kMutex_t mutex0, mutex1;
kSemaphore_t semaphore0;
kTaskHandle_t t1, t2, t3;

kFifo_t queue0;

kTask simpleTask(void* args)
{
	kernel_yield(1500);
	debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Job 1 starts, creating queue\r\n"));

	char buff[32];
	char hello[] = "Hello from task1!";
	uint8_t hellosize = strlen(hello);
	queue0 = threads_fifoInit(buff, 32);

	kTaskHandle_t handle = kernel_getCurrentTaskHandle();

	while (1) {
		uint16_t startTime = kernel_getUptime();
		threads_mutexLock(&mutex0);

		//debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Job 1 runs, sending character to queue\r\n"));
		for (int i = 0; i < hellosize; i++) {
			if (threads_fifoWrite(&queue0, hello[i])) {
				//debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Fifo write success, written character = %c\r\n"), hello[i]);
			}
			else {
				//debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Fifo write error\r\n"));
			}
		}
		//*(handle -> stackBegin - handle -> stackSize - 5) = 99;
		//debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Job 1 ends\r\n"));
		threads_mutexUnlock(&mutex0);

		uint16_t endTime = kernel_getUptime();
		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Job 1 exec time: %d\r\n"), endTime-startTime);
		kernel_yield(20);
		break;
	}
	return; //Oops!
}

kTask simpleTask1(void* args)
{
	kernel_yield(1500);
	debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Job 2 starts\r\n"));
	while (1) {
		char hello1[32] = "";
		threads_mutexLock(&mutex0);
		uint8_t pos = 0;
		while (threads_fifoAvailable(&queue0)) {
			hello1[pos] = threads_fifoRead(&queue0);
			pos++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Fifo contents: %s\r\n"), hello1);

		//debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Job 2 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		kernel_yield(50);
	}
}

kTask simpleTask2(void* args)
{
	kernel_yield(1500);
	debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Job 3 starts\r\n"));
	char* hello1 = (char*)args;
	uint8_t hellosize = strlen(hello1);
	while (1) {
		//debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Job 3 runs, sending character to queue\r\n"));
		threads_mutexLock(&mutex0);
		for (int i = 0; i < hellosize; i++) {
			if (threads_fifoWrite(&queue0, hello1[i])) {
				//debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Fifo write success, written character = %c\r\n"), hello1[i]);
			}
			else {
				//debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Fifo write error\r\n"));
			}
		}

		//debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Job 3 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		kernel_yield(10);
		threads_exitCriticalSection();
	}
}
kTask simpleTask3(void* args)
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 4 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 4 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
	}
}
kTask simpleTask4(void* args)
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 5 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 5 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask5(void* args)
{
	while (1) {
		threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 6 starts, waiting for semaphore\r\n"));
		kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 6 ends, signaling semaphore\r\n"));
		threads_semaphoreSignal(&semaphore0);
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

void simpleService()
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("Example systemd service\r\n"));
}
void simpleService1()
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("Example systemd service 1\r\n"));
}

void simpleService2()
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("Example systemd service 2\r\n"));
}

void user_preinit()
{
	return;
}

void user_init()
{
	debug_puts(L_INFO, PSTR("kernel: Starting systemd process\r\n"));
	systemd_init();
	static char test[] = "test arg string";
	systemd_addService(SDSERVICE_REPEATED, simpleService, 500, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_REPEATED, simpleService1, 200, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_REPEATED, simpleService2, 750, SDSTATE_ACTIVE);

	mutex0 = threads_mutexInit();
	semaphore0 = threads_semaphoreInit(2);

	kernel_createTask(simpleTask, NULL, 250, 5, KTASK_USER, "task1");
	kernel_createTask(simpleTask1, NULL, 250, 4, KTASK_USER, "task2");
	kernel_createTask(simpleTask2, (void*)test, 250, 2, KTASK_USER, "task3");
	return;
}

void user_postinit()
{
	return;
}

int main()
{
	initd_startup();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}
