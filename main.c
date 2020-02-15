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
		debug_puts(L_NONE, PSTR("Job 1 starts\r\n"));
		resourse = 2;
		kernel_yield(100);
		debug_puts(L_NONE, PSTR("Job 1 ends\r\n"));
		kernel_yield(100);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}
kTask simpleTask1()
{
	while (1) {
		debug_puts(L_NONE, PSTR("Job 2 starts\r\n"));
		resourse += 2;
		_delay_ms(200);
		debug_puts(L_NONE, PSTR("Job 2 ends\r\n"));
		_delay_ms(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

kTask simpleTask2()
{
	while (1) {
		debug_puts(L_NONE, PSTR("Job 3 starts\r\n"));
		resourse *= 3;
		_delay_ms(200);
		debug_puts(L_NONE, PSTR("Job 3 ends\r\n"));
		_delay_ms(200);
		//kernel_setTaskState(kernel_getCurrentTaskHandle(), KSTATE_SUSPENDED);
		//while(1);
	}
}

int main()
{
	mutex0 = kernel_createMutex();
	
	t1 = kernel_createTask(simpleTask, 256, 3, KTASK_USER);
	t2 = kernel_createTask(simpleTask1, 256, 3, KTASK_USER);
	t3 = kernel_createTask(simpleTask2, 256, 3, KTASK_USER);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}