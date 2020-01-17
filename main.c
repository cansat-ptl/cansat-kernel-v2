/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: IKS2019
 */ 

#include "config.h"
#include <avr/io.h>

kTaskHandle_t t1, t2, t3;

void simpleTask()
{
	while (1) {
		kernel_disableContextSwicth();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 1)!!! (low priority)\r\n"));
		kernel_enableContextSwicth();
		_delay_ms(10);
	}
}
void simpleTask1()
{
	while (1) {
		kernel_disableContextSwicth();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 2)!!! (high priority)\r\n"));
		//hal_DELAY_MS(50);
		kernel_setTaskState(t2, KSTATE_SUSPENDED);
		kernel_enableContextSwicth();
		_delay_ms(10);
	}
}

void simpleTask2()
{
	while (1) {
		kernel_disableContextSwicth();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 3)!!! (normal priority)\r\n"));
		kernel_enableContextSwicth();
		_delay_ms(10);
	}
}

int main()
{
	t1 = kernel_createTask(simpleTask, 100, KPRIO_HIGH, KTASK_DEFAULT, 10);
	t2 = kernel_createTask(simpleTask1, 100, KPRIO_HIGH, KTASK_DEFAULT, 10);
	t3 = kernel_createTask(simpleTask2, 100, KPRIO_HIGH, KTASK_DEFAULT, 10);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}