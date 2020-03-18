/*
 * signatures.c
 *
 * Created: 18.03.2020 16:50:08
 *  Author: Admin
 */ 

#include <boot/boot.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>

const char boot_msg_mcu[] PROGMEM = "boot: MCU vendor: %s\r\n";
const char boot_msg_mcumem[] PROGMEM = "boot: MCU flash memory: %d KB\r\n";
const char boot_msg_mcurev[] PROGMEM = "boot: MCU Revision: %d\r\n\r\n";

void boot_checkSignature(uint8_t s0, uint8_t s1, uint8_t s2) 
{
	switch (s0) 
	{
		case 0x1E:
			boot_logMessage_p(0, pgm_get_far_address(boot_msg_mcu), "Atmel AVR");
			break;
		default:
			boot_logMessage_p(0, pgm_get_far_address(boot_msg_mcu), "Unknown");
			break;
	}
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_mcumem), (1 << (s1-0x90)));
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_mcurev), s2);
}