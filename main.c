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
#include <initd/initd.h>
uint8_t resourse = 1;

kMutex_t mutex0, mutex1;
kSemaphore_t semaphore0;
kTaskHandle_t t1, t2, t3;

kFifo_t queue0;

kTask simpleTask(void* args)
{
	while (1) {
		//threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 5 starts, waiting for semaphore\r\n"));
		//kernel_yield(200);
		_delay_ms(100);
		debug_puts(L_INFO, PSTR("Job 5 ends, signaling semaphore\r\n"));
		//threads_semaphoreSignal(&semaphore0);
		_delay_ms(100);
		taskmgr_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask1(void* args)
{
	taskmgr_yield(1500);
	while (1) {
		//threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 6 starts, waiting for semaphore\r\n"));
		//kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 6 ends, signaling semaphore\r\n"));
		//threads_semaphoreSignal(&semaphore0);
		_delay_ms(1);
		taskmgr_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

kTask simpleTask2(void* args)
{
	taskmgr_yield(1500);
	while (1) {
		//threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 7 starts, waiting for semaphore\r\n"));
		//kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 7 ends, signaling semaphore\r\n"));
		//threads_semaphoreSignal(&semaphore0);
		_delay_ms(1);
		taskmgr_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

kTask simpleTask3(void* args)
{
	taskmgr_yield(1500);
	while (1) {
		//threads_semaphoreWait(&semaphore0);
		debug_puts(L_INFO, PSTR("Job 8 starts, waiting for semaphore\r\n"));
		//kernel_yield(200);
		debug_puts(L_INFO, PSTR("Job 8 ends, signaling semaphore\r\n"));
		//threads_semaphoreSignal(&semaphore0);
		_delay_ms(1);
		taskmgr_yield(200);
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
	//kernel_createTask(simpleTask3, NULL, 250, 5, KTASK_USER, "task1");
	//kernel_createTask(simpleTask4, NULL, 250, 5, KTASK_USER, "task2");
	//kernel_createTask(simpleTask5, NULL, 250, 4, KTASK_USER, "task1");
	//kernel_createTask(simpleTask6, NULL, 250, 4, KTASK_USER, "task2");
	//kernel_createTask(simpleTask7, NULL, 250, 4, KTASK_USER, "task1");
	return;
}

void user_postinit()
{
	taskmgr_createTask(simpleTask, NULL, 250, 2, KTASK_USER, "task1");
	taskmgr_createTask(simpleTask1, NULL, 250, 4, KTASK_USER, "task2");
	taskmgr_createTask(simpleTask2, NULL, 250, 2, KTASK_USER, "task3");
	return;
}

int main()
{
	initd_startup();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}
