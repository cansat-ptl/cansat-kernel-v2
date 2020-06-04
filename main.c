/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */

#include "config.h"
#include <math.h>
#include <kernel/kernel.h>
#include <avr/io.h>
#include <systemd/systemd.h>
uint8_t resourse = 1;

kMutex_t mutex0, mutex1;
kSemaphore_t semaphore0;
kTaskHandle_t t1, t2, t3, t4;

kFifo_t queue0;

kTask simpleTask(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 1 locks mutex\r\n"));
		//threads_mutexLock(&mutex0);
		debug_puts(L_INFO, PSTR("Task 1 executes\r\n"));
		taskmgr_yield(1000);
		debug_puts(L_INFO, PSTR("Task 1 unlocks mutex\r\n"));
		//threads_mutexUnlock(&mutex0);
		taskmgr_yield(1000);
	}
}

kTask simpleTask1(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 2 locks mutex\r\n"));
		//threads_mutexLock(&mutex0);
		debug_puts(L_INFO, PSTR("Task 2 executes\r\n"));
		taskmgr_yield(1000);
		debug_puts(L_INFO, PSTR("Task 2 unlocks mutex\r\n"));
		//threads_mutexUnlock(&mutex0);
		taskmgr_yield(1000);
	}
}

kTask simpleTask2(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 3 locks mutex\r\n"));
		//threads_mutexLock(&mutex0);
		debug_puts(L_INFO, PSTR("Task 3 executes\r\n"));
		taskmgr_yield(1000);
		debug_puts(L_INFO, PSTR("Task 3 unlocks mutex\r\n"));
		//threads_mutexUnlock(&mutex0);
		taskmgr_yield(1000);
	}
}

kTask simpleTask3(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 4 locks mutex\r\n"));
		//threads_mutexLock(&mutex0);
		debug_puts(L_INFO, PSTR("Task 4 executes\r\n"));
		taskmgr_yield(1000);
		debug_puts(L_INFO, PSTR("Task 4 unlocks mutex\r\n"));
		//threads_mutexUnlock(&mutex0);
		taskmgr_yield(1000);
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
	systemd_addService(SDSERVICE_REPEATED, simpleService, 100, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_REPEATED, simpleService1, 200, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_REPEATED, simpleService2, 300, SDSTATE_ACTIVE);

	mutex0 = threads_mutexInit();
	semaphore0 = threads_semaphoreInit(2);
	//kernel_createTask(simpleTask3, NULL, 250, 5, KTASK_USER, "task1");
	//kernel_createTask(simpleTask4, NULL, 250, 5, KTASK_USER, "task2");
	//kernel_createTask(simpleTask5, NULL, 250, 4, KTASK_USER, "task1");
	//kernel_createTask(simpleTask6, NULL, 250, 4, KTASK_USER, "task2");
	//kernel_createTask(simpleTask7, NULL, 250, 4, KTASK_USER, "task1");
	return;
}

void user_postinit()
{
	t1 = taskmgr_createTask(simpleTask, NULL, 128, 5, KTASK_USER, "task1");
	t2 = taskmgr_createTask(simpleTask1, NULL, 128, 5, KTASK_USER, "task2");
	t3 = taskmgr_createTask(simpleTask2, NULL, 128, 5, KTASK_USER, "task3");
	t4 = taskmgr_createTask(simpleTask3, NULL, 128, 5, KTASK_USER, "task4");
	return;
}

int main()
{
	kernel_startup();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}
