/*
 * uart.h
 *
 * Created: 13.02.2020 19:40:49
 *  Author: Admin
 */ 


#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 8000000L						//CPU frequency
#endif

#include <kernel/types.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

void basicUart_init(unsigned int ubrr);
void basicUart_putc(char c);
void basicUart_puts(char * msg);
void basicUart_enableInterruptsRX();
void basicUart_disableInterruptsRX();

#endif /* UART_H_ */