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

char exampleBuffer[32];
static char exampleParameter[] = "Spaghetti and meatballs\0";
kFifoHandle_t exampleFifo;
kMutex_t exampleMutex;
kTaskHandle_t t1, t2, t3;

kTask simpleTask1(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Task 1 starts\r\n"));
	
	while (1) {
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Hello from task 1!\r\n"));

		threads_mutexLock(&exampleMutex);
		while (threads_fifoAvailable(exampleFifo)) {
			threads_fifoRead(exampleFifo, (void*)&receiveBuffer[receiveBufferIndex]);
			receiveBufferIndex++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Fifo contents: %s\r\n"), receiveBuffer);
		threads_mutexUnlock(&exampleMutex);

		taskmgr_sleep(200);
	}
}

kTask simpleTask2(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Task 2 starts\r\n"));
	char receiveBuffer[32];
	
	while (1) {
		uint8_t receiveBufferIndex = 0;

		debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Hello from task 2!\r\n"));

		threads_mutexLock(&exampleMutex);
		while (threads_fifoAvailable(exampleFifo)) {
			threads_fifoRead(exampleFifo, (void*)&receiveBuffer[receiveBufferIndex]);
			receiveBufferIndex++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Fifo contents: %s\r\n"), receiveBuffer);
		threads_mutexUnlock(&exampleMutex);

		taskmgr_sleep(200);
	}
}

kTask simpleTask3(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Task 3 starts\r\n"));

	while (1) {
		debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Hello from task 3! My favorite food is %s\r\n"), (char*)args);
		debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Telling this to everyone\r\n"), (char*)args);

		threads_mutexLock(&exampleMutex);
		for (int i = 0; i < strlen((char*)args); i++) {
			if (threads_fifoWrite(exampleFifo, &((char*)args)[i])) {
				debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Fifo write error\r\n"));
			}
		}
		threads_mutexUnlock(&exampleMutex);

		taskmgr_sleep(200);
	}
}

void user_preinit()
{
	exampleFifo = threads_fifoCreate(1, 32);
	exampleMutex = threads_mutexInit(); //Initializes mutex
	return;
}

void user_init()
{
	t1 = taskmgr_createTask(simpleTask1, NULL, 250, 1, KTASK_USER, "task1");
	t2 = taskmgr_createTask(simpleTask2, NULL, 250, 2, KTASK_USER, "task2");
	t3 = taskmgr_createTask(simpleTask3, (void*)exampleParameter, 250, 3, KTASK_USER, "task3");
	return;
}

void user_postinit()
{
	//Nothing to do in post-init
	return;
}

int main()
{
	kernel_startup(); //Call initd_startup to run the kernel
	while (1);
}