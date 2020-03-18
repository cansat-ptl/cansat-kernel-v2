/*
 * yktsat-boot.c
 *
 * Created: 05.03.2020 19:43:09
 * Author : Admin
 */ 

#include <boot/boot.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>

const char msg0[] PROGMEM = "boot: test string %d %d\r\n";


int main(void)
{
	basicUart_init(12);
	uint8_t a = 12, b = 34;
	boot_logMessage_p(0, pgm_get_far_address(msg0), a, b);
    while(1);
    ((void(*)(void))0)();
}

