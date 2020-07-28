/*
 * uart.h
 *
 * Created: 28.07.2020 21:32:23
 *  Author: Admin
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void basicUart_init(uint16_t ubrr);
void basicUart_putc(char c);
void basicUart_puts(char * msg);
void basicUart_enableInterruptsRX();
void basicUart_disableInterruptsRX();

#endif /* UART_H_ */