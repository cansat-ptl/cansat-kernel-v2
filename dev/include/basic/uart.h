/*
 * uart.h
 *
 * Created: 13.02.2020 19:40:49
 *  Author: Admin
 */ 


#ifndef UART_H_
#define UART_H_

void basicUart_init(unsigned int ubrr);
void basicUart_putc(char c);
void basicUart_puts(char * msg);
void basicUart_enableInterruptsRX();
void basicUart_disableInterruptsRX();

#endif /* UART_H_ */