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

kMutex_t mutex0;
kTaskHandle_t t1, t2, t3;

kTask simpleTask()
{
	while (1) {
		kernel_lockMutex(&mutex0);
		debug_puts(L_NONE, PSTR("Job 1 starts, accessing resourse\r\n"));
		resourse = 2;
		kernel_yield(500);
		debug_puts(L_NONE, PSTR("Job 1 ends, accessing resourse\r\n"));
		kernel_unlockMutex(&mutex0);
		kernel_yield(500);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask1()
{
	while (1) {
		kernel_lockMutex(&mutex0);
		debug_puts(L_NONE, PSTR("Job 2 starts, accessing resourse\r\n"));
		resourse += 2;
		kernel_yield(600);
		debug_puts(L_NONE, PSTR("Job 2 ends, accessing resourse\r\n"));
		kernel_unlockMutex(&mutex0);
		kernel_yield(600);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

kTask simpleTask2()
{
	while (1) {
		kernel_lockMutex(&mutex0);
		debug_puts(L_NONE, PSTR("Job 3 starts, accessing resourse\r\n"));
		resourse *= 3;
		kernel_yield(400);
		debug_puts(L_NONE, PSTR("Job 3 ends, accessing resourse\r\n"));
		kernel_unlockMutex(&mutex0);
		kernel_yield(400);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask3()
{
	while (1) {
		debug_puts(L_NONE, PSTR("Job 4 starts\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 4 ends\r\n"));
		kernel_yield(200);
	}
}
kTask simpleTask4()
{
	while (1) {
		debug_puts(L_NONE, PSTR("Job 5 starts\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 5 ends\r\n"));
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask5()
{
	while (1) {
		debug_puts(L_NONE, PSTR("Job 6 starts\r\n"));
		kernel_yield(200);
		debug_puts(L_NONE, PSTR("Job 6 ends\r\n"));
		kernel_yield(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

int main()
{
	mutex0 = kernel_createMutex();
	
	kernel_createTask(simpleTask, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask1, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask2, 150, 3, KTASK_USER);
	kernel_createTask(simpleTask3, 150, 2, KTASK_USER);
	kernel_createTask(simpleTask4, 150, 2, KTASK_USER);
	kernel_createTask(simpleTask5, 150, 1, KTASK_USER);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}