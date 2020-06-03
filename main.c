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
kTaskHandle_t t1, t2, t3;

kFifo_t queue0;

kTask simpleTask(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 1 executes\r\n"));
		taskmgr_yield(100);
		_delay_ms(10);
	}
}

kTask simpleTask1(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 2 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask2(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 3 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask3(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 4 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask4(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 5 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask5(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 6 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}
kTask simpleTask6(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 7 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask7(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 8 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask8(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 9 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask9(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 10 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask10(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 11 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
	}
}

kTask simpleTask11(void* args)
{
	while (1) {
		debug_puts(L_INFO, PSTR("Task 12 executes\r\n"));
		taskmgr_yield(200);
		_delay_ms(10);
		debug_logMessage(PGM_ON, L_INFO, PSTR("Removing tasks\r\n"));
			taskmgr_removeTask(t1);
			taskmgr_removeTask(t2);
			taskmgr_removeTask(t3);
			while(1);
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
	//debug_logMessage(PGM_ON, L_INFO, PSTR("Removing tasks from systemd service\r\n"));
}

void user_preinit()
{
	return;
}

void user_init()
{
	debug_puts(L_INFO, PSTR("kernel: Starting systemd process\r\n"));
	systemd_init();
	//static char test[] = "test arg string";
	systemd_addService(SDSERVICE_REPEATED, simpleService, 100, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_REPEATED, simpleService1, 200, SDSTATE_ACTIVE);
	systemd_addService(SDSERVICE_SINGLERUN, simpleService2, 5000, SDSTATE_ACTIVE);

	mutex0 = threads_mutexInit();
	semaphore0 = threads_semaphoreInit(2);
	return;
}

void user_postinit()
{
	taskmgr_createTask(simpleTask, NULL, 128, 2, KTASK_USER, "task1");
	taskmgr_createTask(simpleTask1, NULL, 128, 2, KTASK_USER, "task2");
	taskmgr_createTask(simpleTask2, NULL, 128, 2, KTASK_USER, "task3");
	taskmgr_createTask(simpleTask3, NULL, 128, 2, KTASK_USER, "task4");
	taskmgr_createTask(simpleTask4, NULL, 128, 3, KTASK_USER, "task5");
	taskmgr_createTask(simpleTask5, NULL, 128, 2, KTASK_USER, "task6");
	taskmgr_createTask(simpleTask6, NULL, 128, 4, KTASK_USER, "task7");
	t3 = taskmgr_createTask(simpleTask7, NULL, 128, 7, KTASK_USER, "task8");
	t2 = taskmgr_createTask(simpleTask8, NULL, 128, 7, KTASK_USER, "task9");
	t1 = taskmgr_createTask(simpleTask9, NULL, 128, 7, KTASK_USER, "task10");
	taskmgr_createTask(simpleTask10, NULL, 128, 2, KTASK_USER, "task11");
	taskmgr_createTask(simpleTask11, NULL, 128, 2, KTASK_USER, "task12");
	return;
}

int main()
{
	kernel_startup();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}
