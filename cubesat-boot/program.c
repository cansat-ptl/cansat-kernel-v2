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

static volatile uint16_t cnt = 0;
static volatile uint16_t tmp = 0;

const char boot_msg_recv[] PROGMEM = "\r\nboot: Recv = %c\r\n";
const char boot_msg_int[] PROGMEM = "\r\nboot: Interrupted\r\n";
const char boot_msg_ok[] PROGMEM = "\r\nok\r\n";

void boot_runStateMachine()
{
	while(1) {
		switch (bCurState) {
			case 3:
				boot_programPage(bActivePage, (uint8_t*)bPageBuffer);
				bCurState = 0;
				bActivePage = 0;
				bPageBufferIndex = 0;
				boot_logMessage_p(0, pgm_get_far_address(boot_msg_ok));
				break;
		}
	}
}

void boot_setActivePage(unsigned char c)
{
	uint32_t stmp = boot_convertAsciiToNumeric(c);
	if (cnt != 7) {
		bActivePage |= ((uint32_t)stmp << (4*(7-cnt)));
		cnt++;
	}
	else {
		bActivePage |= (uint32_t)stmp;
		cnt = 0;
		bCurState = 0;
		boot_logMessage_p(0, pgm_get_far_address(boot_msg_ok));
	}
}

void boot_fillPageBuffer(unsigned char c)
{
	if (cnt == 0) {
		tmp |= ((uint8_t)boot_convertAsciiToNumeric(c) << 4);
		cnt++;
	}
	else {
		cnt = 0;
		tmp |= boot_convertAsciiToNumeric(c);
		if (bPageBufferIndex < SPM_PAGESIZE-1) {
			bPageBuffer[bPageBufferIndex] = tmp;
			bPageBufferIndex++;
		}
		else {
			bPageBuffer[SPM_PAGESIZE-1] = tmp;
			bCurState = 0;
			boot_logMessage_p(0, pgm_get_far_address(boot_msg_ok));
		}
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
					boot_logMessage_p(0, pgm_get_far_address(boot_msg_recv), c);
					break;
				case 's':
					bCurState = 2;
					boot_logMessage_p(0, pgm_get_far_address(boot_msg_recv), c);
					break;
				case 'w':
					bCurState = 3;
					boot_logMessage_p(0, pgm_get_far_address(boot_msg_recv), c);
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
	if (c == 'r') {
		bCurState = 0;
		cnt = 0;
		tmp = 0;
		boot_logMessage_p(0, pgm_get_far_address(boot_msg_int));
	}
}

ISR(USART0_RX_vect)
{
	unsigned char c = UDR0;
	boot_processCommand(c);
}