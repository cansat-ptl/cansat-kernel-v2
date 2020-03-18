/*
 * program.c
 *
 * Created: 18.03.2020 21:07:18
 *  Author: Admin
 */ 

#include <boot/boot.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>

volatile uint8_t bPageBuffer[SPM_PAGESIZE];
volatile uint16_t bPageBufferIndex = 0;

volatile uint32_t bActivePage = 0;

volatile uint8_t bCurState = 0;

void boot_runStateMachine() 
{
	while(1) {
		switch (bCurState) {
			case 3:
				boot_programPage(bActivePage, (uint8_t*)bPageBuffer);
				bCurState = 0;
				break;
		}
	}
}

void boot_setActivePage(unsigned char c)
{
	static uint8_t cnt = 0;
	if (cnt < 3) {
		bActivePage |= ((uint32_t)c << (8*cnt));
		cnt++;
	}
	else {
		cnt = 0;
		bCurState = 0;
	}
}

void boot_fillPageBuffer(unsigned char c) 
{
	if (bPageBufferIndex < SPM_PAGESIZE) {
		bPageBuffer[bPageBufferIndex] = (uint8_t)c;
		bPageBufferIndex++;
	}
	else {
		bCurState = 0;
	}
}

static inline void boot_processCommand(unsigned char c)
{
	switch (bCurState) {
		case 0:
			switch (c) {
				case 'a':
					bCurState = 1;
					break;
				case 'b':
					bCurState = 2;
					break;
				case 'c':
					bCurState = 3;
					break;
				default:
					bCurState = 0;
					break;
			}
			break;
		case 1:
			boot_setActivePage(c-'0');
			break;
		case 2:
			boot_fillPageBuffer(c-'0');
			break;
	}
}

ISR(USART0_RX_vect) 
{
	unsigned char c = UDR0;
	boot_processCommand(c);
}