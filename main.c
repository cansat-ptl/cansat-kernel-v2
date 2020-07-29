/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */
#include <math.h>
#include <avr/io.h>
#include <kernel/kernel.h>
#include <kdebug/debug.h>

char exampleBuffer[32];
static char exampleParameter[] = "Spaghetti and meatballs\0";
kFifoHandle_t exampleFifo;
kMutexHandle_t exampleMutex;
kTaskHandle_t handles[4];

kTask simpleTask1(void* args)
{
	//kTaskHandle_t handle = tasks_getCurrentTaskHandle();

	while (1) {
		debug_logMessage(PGM_PUTS, L_INFO, PSTR("task1: Waiting for notification\r\n"));
		uint16_t flags = tasks_notificationWait();
		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Notification received, flags = 0x%04X\r\n"), flags);
	}
}

kTask simpleTask2(void* args)
{
	tasks_sleep(1000);
	while (1) {
		debug_logMessage(PGM_PUTS, L_INFO, PSTR("task2: Sending notification\r\n"));
		tasks_notificationSend(((kTaskHandle_t*)args)[0], 0x1234);
		tasks_notificationSend(((kTaskHandle_t*)args)[2], 0x4321);
		tasks_notificationSend(((kTaskHandle_t*)args)[3], 0x1111);
		tasks_sleep(1000);
	}
}

kTask simpleTask3(void* args)
{
	while (1) {
		debug_logMessage(PGM_PUTS, L_INFO, PSTR("task3: Waiting for notification\r\n"));
		uint16_t flags = tasks_notificationWait();
		debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Notification received, flags = 0x%04X\r\n"), flags);
		tasks_sleep(200);
	}
}

kTask simpleTask4(void* args)
{
	while (1) {
		debug_logMessage(PGM_PUTS, L_INFO, PSTR("task4: Waiting for notification\r\n"));
		uint16_t flags = tasks_notificationWait();
		debug_logMessage(PGM_ON, L_INFO, PSTR("task4: Notification received, flags = 0x%04X\r\n"), flags);
	}
}

void user_preinit()
{
	exampleFifo = threads_fifoCreate(1, 32);
	exampleMutex = threads_mutexCreate(); //Initializes mutex
	return;
}

void user_init()
{
	handles[0] = tasks_createTask(simpleTask1, NULL, 250, 2, KTASK_USER, "task1");
	handles[1] = tasks_createTask(simpleTask2, (void*)handles, 250, 2, KTASK_USER, "task2");
	handles[2] = tasks_createTask(simpleTask3, NULL, 250, 2, KTASK_USER, "task3");
	handles[3] = tasks_createTask(simpleTask4, NULL, 250, 2, KTASK_USER, "task4");
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