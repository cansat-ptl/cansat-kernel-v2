/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */
#include <math.h>
#include <avr/io.h>
#include <systemd/systemd.h>
#include <kernel/kernel.h>
#include <kernel/types.h>

kMutex_t mutex0, mutex1;
kSemaphore_t semaphore0;
kSpinlock_t spinlock0;
kTaskHandle_t t1, t2, t3;
volatile char buff[32];

kFifo_t queue0;

kTask simpleTask1(void* args)
{
	//taskmgr_yield(1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("Job 1 starts, creating queue\r\n"));

	char hello[] = "Hello from task1!";
	uint8_t hellosize = strlen(hello);

	while (1) {
		uint16_t startTime = kernel_getUptime();
		threads_mutexLock(&mutex0);
		debug_logMessage(PGM_ON, L_INFO, PSTR("Job 1 runs, sending character to queue\r\n"));
		for (int i = 0; i < hellosize; i++) {
			if (threads_fifoWrite(&queue0, hello[i])) {
				debug_logMessage(PGM_ON, L_INFO, PSTR("Fifo write success, written character = %c\r\n"), hello[i]);
			}
			else {
				debug_logMessage(PGM_ON, L_INFO, PSTR("Fifo write error\r\n"));
			}
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("Job 1 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		uint16_t endTime = kernel_getUptime();
		debug_logMessage(PGM_ON, L_INFO, PSTR("Job 1 exec time: %d\r\n"), endTime-startTime);
		taskmgr_yield(200);
	}
}

kTask simpleTask2(void* args)
{
	//taskmgr_yield(1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("Job 2 starts\r\n"));
	char hello1[32];
	while (1) {
		threads_mutexLock(&mutex0);
		uint8_t pos = 0;
		while (threads_fifoAvailable(&queue0)) {
			hello1[pos] = threads_fifoRead(&queue0);
			pos++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("Lifo contents: %s\r\n"), hello1);
		
		debug_logMessage(PGM_ON, L_INFO, PSTR("Job 2 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		taskmgr_yield(200);
	}
}

kTask simpleTask3(void* args)
{
	//taskmgr_yield(1);
	debug_logMessage(PGM_ON, L_INFO, PSTR("Job 2 starts\r\n"));
	char hello1[32];
	while (1) {
		threads_mutexLock(&mutex0);
		uint8_t pos = 0;
		while (threads_fifoAvailable(&queue0)) {
			hello1[pos] = threads_fifoRead(&queue0);
			pos++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("Lifo contents: %s\r\n"), hello1);
		
		debug_logMessage(PGM_ON, L_INFO, PSTR("Job 2 ends\r\n"));
		threads_mutexUnlock(&mutex0);
		taskmgr_yield(200);
	}
}


void user_preinit()
{
	threads_fifoCreateStatic(&queue0, buff, 1, 32);
	mutex0 = threads_mutexInit(); //Initializes mutex
	return;
}

void user_init()
{
	t1 = taskmgr_createTask(simpleTask1, NULL, 350, 4, KTASK_USER, "task1");
	t2 = taskmgr_createTask(simpleTask2, NULL, 350, 3, KTASK_USER, "task2");
	t3 = taskmgr_createTask(simpleTask3, NULL, 350, 1, KTASK_USER, "task3");
	return;
}

void user_postinit()
{
	return;
}

int main()
{
	kernel_startup(); //Call initd_startup to run the kernel
	while (1);
}
