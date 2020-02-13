/*
 * uart.h
 *
 * Created: 13.02.2020 19:40:49
 *  Author: Admin
 */ 


#ifndef UART_H_
#define UART_H_

#ifndef hal_UART_INIT
	#define hal_UART_INIT(ubrr) hal_basicUart_init(ubrr)
	void hal_basicUart_init(unsigned int ubrr);
#endif

#ifndef hal_UART_PUTC
	#define hal_UART_PUTC(c) hal_basicUart_putc(c)
	void hal_basicUart_putc(char c);
#endif

#ifndef hal_UART_PUTS
	#define hal_UART_PUTS(s) hal_basicUart_puts(s)
	void hal_basicUart_puts(char * msg);
#endif

#ifndef hal_UART_ENABLE_RX_INT
	#define hal_UART_ENABLE_RX_INT() hal_basicUart_enableInterruptsRX()
	void hal_basicUart_enableInterruptsRX();
#endif

#ifndef hal_UART_DISABLE_RX_INT
	#define hal_UART_DISABLE_RX_INT(s) hal_basicUart_disableInterruptsRX()
	void hal_basicUart_disableInterruptsRX();
#endif

#endif /* UART_H_ */