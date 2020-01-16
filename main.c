/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: IKS2019
 */ 

#include "config.h"
#include <avr/io.h>
void simpleTask()
{
	while (1) {
		//hal_disableInterrupts();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 1)!!! (low priority)\r\n"));
		//hal_enableInterrupts();
		//delay_ms(1);
	}
}
void simpleTask1()
{
	while (1) {
		//hal_disableInterrupts();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 2)!!! (high priority)\r\n"));
		//hal_enableInterrupts();
		//delay_ms(1);
	}
}

void simpleTask2()
{
	while (1) {
		//hal_disableInterrupts();
		debug_puts(L_NONE, PSTR("Holy shit it fuckin works (task 3)!!! (normal priority)\r\n"));
		//hal_enableInterrupts();
		//delay_ms(1);
	}
}

int main()
{
	kernel_createTask(simpleTask, 100, KPRIO_LOW, KTASK_DEFAULT, 1);
	kernel_createTask(simpleTask1, 100, KPRIO_HIGH, KTASK_DEFAULT, 1);
	kernel_createTask(simpleTask2, 100, KPRIO_NORM, KTASK_DEFAULT, 1);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}