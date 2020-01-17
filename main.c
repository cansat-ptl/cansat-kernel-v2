/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: IKS2019
 */ 

#include "config.h"
#include <math.h>
#include <avr/io.h>

kTaskHandle_t t1, t2, t3;

void simpleTask()
{
	while (1) {
		char ac[64] = "Ight Imma Head Out\r\n";
		kernel_disableContextSwicth();
		debug_logMessage(PGM_ON, L_NONE, PSTR("It works! IT WORKS! (task 1)(high priority)\r\n"));
		kernel_enableContextSwicth();
		debug_puts(L_NONE, PSTR("Testing non-atomic uart output... asdfasgrtertgawe2345\r\n"));
		float a = 0.1252, b = 23.567, c = 3423.2341, d = 342.1234, e, f, h;
		e = sin(c);
		f = asin(e);
		h = (pow(a, 2) * pow(b, 3) - c)/e + f;
		d = log(d);
		kernel_disableContextSwicth();
		debug_logMessage(PGM_ON, L_NONE, PSTR("Math test: h = %f, e = %f, f = %f, d = %f\r\n"), h, e, f, d);
		kernel_enableContextSwicth();
		debug_logMessage(PGM_OFF, L_NONE, ac);
		_delay_ms(10);
	}
}
void simpleTask1()
{
	while (1) {
		kernel_disableContextSwicth();
		debug_puts(L_NONE, PSTR("It works! IT WORKS! (task 2)(high priority)\r\n"));
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
		debug_puts(L_NONE, PSTR("It works! IT WORKS! (task 2)(high priority)\r\n"));
		kernel_enableContextSwicth();
		_delay_ms(10);
	}
}

int main()
{
	t1 = kernel_createTask(simpleTask, 400, KPRIO_HIGH, KTASK_DEFAULT, 10);
	t2 = kernel_createTask(simpleTask1,64, KPRIO_HIGH, KTASK_DEFAULT, 10);
	t3 = kernel_createTask(simpleTask2, 64, KPRIO_HIGH, KTASK_DEFAULT, 10);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}