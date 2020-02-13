/*
 * watchdog.c
 *
 * Created: 19.01.2020 16:55:00
 *  Author: ThePetrovich
 */ 
#include <kernel.h>

uint8_t mcucsr_mirror __attribute__ ((section (".noinit")));

void wdt_saveMCUCSR(void)
{
	mcucsr_mirror = MCUCSR;
	MCUCSR = 0;
	wdt_disable();
}

void wdt_enableWatchdog()
{
	//	WDTCR = (1<<WDCE)|(1<<WDE);
	//	WDTCR = 0;
	//	WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP0);
	wdt_enable(WDTO_2S);
}