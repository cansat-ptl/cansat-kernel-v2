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
				bActivePage = 0;
				bPageBufferIndex = 0;
				break;
		}
	}
}

void boot_setActivePage(unsigned char c)
{
	static uint8_t cnt = 8;
	uint32_t tmp = boot_convertAsciiToNumeric(c);
	if (cnt > 0) {
		bActivePage |= ((uint32_t)tmp << (4*(cnt-1)));
		cnt--;
	}
	else {
		cnt = 8;
		bCurState = 0;
	}
}

void boot_fillPageBuffer(unsigned char c)
{
	static uint8_t cnt = 0;
	uint8_t tmp = 0;
	if (cnt == 0) {
		tmp |= (boot_convertAsciiToNumeric(c) << 4);
		cnt++;
	}
	else {
		tmp |= boot_convertAsciiToNumeric(c);
		if (bPageBufferIndex < SPM_PAGESIZE) {
			bPageBuffer[bPageBufferIndex] = (uint8_t)tmp;
			bPageBufferIndex++;
		}
		else {
			bCurState = 0;
		}
		cnt = 0;
	}
}

static inline void boot_processCommand(unsigned char c)
{
	switch (bCurState) {
		case 0:
			switch (c) {
				case 'p':
					bActivePage = 0;
					bCurState = 1;
					break;
				case 's':
					bCurState = 2;
					break;
				case 'w':
					bCurState = 3;
					break;
				default:
					bCurState = 0;
					break;
			}
			break;
		case 1:
			boot_setActivePage(c);
			break;
		case 2:
			boot_fillPageBuffer(c);
			break;
	}
}

ISR(USART0_RX_vect)
{
	unsigned char c = UDR0;
	boot_processCommand(c);
}