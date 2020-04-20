/*
 * hal.c
 *
 * Created: 21.06.2019 23:53:24
 *  Author: ThePetrovich
 */

#include <platform/platform.h>
#include <dev/basic/uart.h>

void basicUart_init (uint16_t ubrr)
{
	UBRR0H = (ubrr >> 8);
	UBRR0L = (ubrr & 0xFF);
	UCSR0A |= (1 << U2X);
	UCSR0B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(0<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
}

void basicUart_putc(char c)
{
	UDR0 = c;
	while (!(UCSR0A & (1<<UDRE)));
}

void basicUart_puts(char * msg)
{
	int i = 0;
	while (msg[i] != '\0') {
		basicUart_putc(msg[i]);
		i++;
	}
}

void basicUart_enableInterruptsRX()
{
	UCSR0B |= (1 << RXCIE);
}

void basicUart_disableInterruptsRX()
{
	UCSR0B &= ~(1 << RXCIE);
}