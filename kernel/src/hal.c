/*
 * hal.c
 *
 * Created: 21.06.2019 23:53:24
 *  Author: ThePetrovich
 */ 

#include "../hal.h"

void hal_setupPins()
{
	PORTA = 0;
	DDRA = 0;
	PORTB = 0;
	DDRB = 0;
	PORTC = 0;
	DDRC = 0;
	PORTD = 0;
	DDRD = 0;
	PORTE = 0;
	DDRE = 0;
	PORTF = 0;
	DDRF = 0;
	DDRG |= (1 << PG3);
	DDRB |= (1 << PD4);
	hal_setPinMode(&JUMPER_DDR, JUMPER_OUT, OUTPUT);
	hal_writePin(&JUMPER_PORT, JUMPER_OUT, HIGH);
	DDRC |= (1 << PC4);
	DDRC |= (1 << PC5);
	DDRC |= (1 << PC6);
	DDRC |= (1 << PC7);
}

//0 - OUTPUT, 1 - INPUT, 2 - INPUT_PULLUP
void hal_pinMode(uint8_t pin, uint8_t value)
{
	uint8_t nvalue = !value;
	if(pin < 8){
		hal_writeBit_m(DDRA, pin, nvalue);
	} else if(pin < 16) {
		hal_writeBit_m(DDRB, (pin - 8), nvalue);
	} else if(pin < 24) {
		hal_writeBit_m(DDRC, (pin - 160), nvalue);
	} else if(pin < 32) {
		hal_writeBit_m(DDRD, (pin - 24), nvalue);
	} else if(pin < 40) {
		hal_writeBit_m(DDRE, (pin - 32), nvalue);
	} else if(pin < 48) {
		hal_writeBit_m(DDRF, (pin - 40), nvalue);
	} else if(pin < 53) {
		hal_writeBit_m(DDRG, (pin - 48), nvalue);
	} else {
		return;
	}
	if(value == INPUT_PULLUP)
		hal_digitalWrite(pin, HIGH);
	return;
}

void hal_digitalWrite(uint8_t pin, uint8_t value)
{
	if(pin < 8){
		hal_writeBit_m(PORTA, pin, value);
		return;
	} else if(pin < 16) {
		hal_writeBit_m(PORTB, (pin - 8), value);
		return;
	} else if(pin < 24) {
		hal_writeBit_m(PORTC, (pin - 16), value);
		return;
	} else if(pin < 32) {
		hal_writeBit_m(PORTD, (pin - 24), value);
		return;
	} else if(pin < 40) {
		hal_writeBit_m(PORTE, (pin - 32), value);
		return;
	} else if(pin < 48) {
		hal_writeBit_m(PORTF, (pin - 40), value);
		return;
	} else if(pin < 53) {
		hal_writeBit_m(PORTG, (pin - 48), value);
		return;
	} else {
		return;
	}	
}

uint8_t hal_digitalRead(uint8_t pin)
{
	if(pin < 8)
		return hal_checkBit_m(PINA, pin);
	else if(pin < 16)
		return hal_checkBit_m(PINB, (pin - 8));
	else if(pin < 24)
		return hal_checkBit_m(PINC, (pin - 16));
	else if(pin < 32)
		return hal_checkBit_m(PIND, (pin - 24));
	else if(pin < 40)
		return hal_checkBit_m(PINE, (pin - 32));
	else if(pin < 48)
		return hal_checkBit_m(PINF, (pin - 40));
	else if(pin < 53)
		return hal_checkBit_m(PING, (pin - 48));
	else {
		return 0xFF;
	}
}

void hal_setupTimer1A(uint8_t prescaler)
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TCCR1B |= (1 << WGM12)|(prescaler << CS10); // prescaler 64 cs11 & cs10 = 1
	TCNT1 = 0;
	OCR1A = 125;
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

void hal_startTimer1A()
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TIMSK |= (1 << OCIE1A);
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

void hal_stopTimer1A()
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TIMSK &= ~(1 << OCIE1A);
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

void hal_setupTimer0(uint8_t prescaler)
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TCCR0 |= (prescaler << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = 125;
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

void hal_startTimer0()
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TIMSK |= (1 << OCIE0);
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

void hal_stopTimer0()
{
	uint8_t sreg = hal_statusReg;
	hal_disableInterrupts();
	TIMSK &= ~(1 << OCIE0);
	hal_enableInterrupts();
	hal_statusReg = sreg;
}

int hal_basicUart_init(unsigned int ubrr)
{
	UBRR0H = 0;
	UBRR0L = 12;
	UCSR0B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(0<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

void hal_basicUart_putc(char c)
{
	UDR0 = c;
	while(!(UCSR0A & (1<<UDRE)));
}

void hal_basicUart_puts(char * msg)
{
	int i = 0;
	while(msg[i] != '\0'){
		UDR0 = msg[i];
		while(!(UCSR0A & (1<<UDRE)));
		i++;
	}
}

void hal_basicUart_enableInterruptsRX()
{
	UCSR0B |= (1 << RXCIE);
}

void hal_basicUart_disableInterruptsRX()
{
	UCSR0B &= ~(1 << RXCIE);
}