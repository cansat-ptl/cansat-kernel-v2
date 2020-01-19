/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */ 

#include "config.h"
#include <math.h>
#include <avr/io.h>

kTaskHandle_t t1, t2, t3;

kTask simpleTask()
{
	while (1) {
		char ac[] = "[testd] task1: Ight Imma Head Out\r\n";
		
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task1: It works! IT WORKS! (high priority)\r\n"));
		debug_puts(L_NONE, PSTR("[testd] task1: Testing non-atomic uart output... asdfasgrtertgawe2345\r\n"));
		kernel_enableContextSwitch();
				
		float a = 0.1252, b = 23.567, c = 3423.2341, d = 342.1234, e, f, h;
		e = sin(c);
		_delay_ms(2);
		f = asin(e);
		_delay_ms(2);
		h = (pow(a, 2) * pow(b, 3) - c)/e + f;
		_delay_ms(2);
		d = log(d);
		_delay_ms(2);
		
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task1: Math test: h = %f, e = %f, f = %f, d = %f\r\n"), h, e, f, d);
		debug_logMessage(PGM_OFF, L_NONE, ac);
		kernel_enableContextSwitch();
	}
}
kTask simpleTask1()
{
	while (1) {
		char ac[] = "[testd] task2: Ight Imma Head Out\r\n";
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task2: It works! IT WORKS! (high priority)\r\n"));
		debug_puts(L_NONE, PSTR("[testd] task2: Testing non-atomic uart output... asdfasgrtertgawe2345\r\n"));
		kernel_enableContextSwitch();
		
		float a = 123.1446, b = 1.43567, c = 477.12431, d = 5.568, e, f, h;
		e = sin(c);
		_delay_ms(2);
		f = asin(e);
		_delay_ms(2);
		h = (pow(a, 2) * pow(b, 3) - c)/e + f;
		_delay_ms(2);
		d = log(d);
		_delay_ms(2);
		
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task2: Math test: h = %f, e = %f, f = %f, d = %f\r\n"), h, e, f, d);
		debug_logMessage(PGM_OFF, L_NONE, ac);
		kernel_enableContextSwitch();
	}
}

kTask simpleTask2()
{
	while (1) {
		char ac[] = "[testd] task3: Ight Imma Head Out\r\n";
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task3: It works! IT WORKS! (high priority)\r\n"));
		debug_puts(L_NONE, PSTR("[testd] task3: Testing non-atomic uart output... asdfasgrtertgawe2345\r\n"));
		kernel_enableContextSwitch();
		
		float a = 2.1252, b = 1.12, c = 3345.132, d = 78.1245, e, f, h;
		e = sin(c);
		_delay_ms(2);
		f = asin(e);
		_delay_ms(2);
		h = (pow(a, 2) * pow(b, 3) - c)/e + f;
		_delay_ms(2);
		d = log(d);
		_delay_ms(2);
		
		kernel_disableContextSwitch();
		debug_logMessage(PGM_ON, L_NONE, PSTR("[testd] task3: Math test: h = %f, e = %f, f = %f, d = %f\r\n"), h, e, f, d);
		debug_logMessage(PGM_OFF, L_NONE, ac);
		kernel_enableContextSwitch();
	}
}

int main()
{
	t1 = kernel_createTask(simpleTask, 256, KPRIO_HIGH, KTASK_USER, 10);
	t2 = kernel_createTask(simpleTask1, 256, KPRIO_HIGH, KTASK_USER, 10);
	t3 = kernel_createTask(simpleTask2, 256, KPRIO_HIGH, KTASK_USER, 10);
	kernel_init();
	//kernel_createTask(simpleTask3, 64, KPRIO_HIGH, KTASK_DEFAULT, 200, "test3");
	while (1);
}