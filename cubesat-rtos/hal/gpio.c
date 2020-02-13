/*
 * io.c
 *
 * Created: 13.02.2020 19:32:11
 *  Author: Admin
 */ 
#include <hal/hal.h>

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
	if (pin < 8) {
		hal_WRITE_BIT(DDRA, pin, nvalue);
	} else if (pin < 16) {
		hal_WRITE_BIT(DDRB, (pin - 8), nvalue);
	} else if (pin < 24) {
		hal_WRITE_BIT(DDRC, (pin - 160), nvalue);
	} else if (pin < 32) {
		hal_WRITE_BIT(DDRD, (pin - 24), nvalue);
	} else if (pin < 40) {
		hal_WRITE_BIT(DDRE, (pin - 32), nvalue);
	} else if (pin < 48) {
		hal_WRITE_BIT(DDRF, (pin - 40), nvalue);
	} else if (pin < 53) {
		hal_WRITE_BIT(DDRG, (pin - 48), nvalue);
	} else {
		return;
	}
	if(value == INPUT_PULLUP)
	hal_digitalWrite(pin, HIGH);
	return;
}

void hal_digitalWrite(uint8_t pin, uint8_t value)
{
	if (pin < 8){
		hal_WRITE_BIT(PORTA, pin, value);
		return;
	} else if (pin < 16) {
		hal_WRITE_BIT(PORTB, (pin - 8), value);
		return;
	} else if (pin < 24) {
		hal_WRITE_BIT(PORTC, (pin - 16), value);
		return;
	} else if (pin < 32) {
		hal_WRITE_BIT(PORTD, (pin - 24), value);
		return;
	} else if (pin < 40) {
		hal_WRITE_BIT(PORTE, (pin - 32), value);
		return;
	} else if (pin < 48) {
		hal_WRITE_BIT(PORTF, (pin - 40), value);
		return;
	} else if (pin < 53) {
		hal_WRITE_BIT(PORTG, (pin - 48), value);
		return;
	} else {
		return;
	}
}

uint8_t hal_digitalRead(uint8_t pin)
{
	if (pin < 8)
	return hal_CHECK_BIT(PINA, pin);
	else if (pin < 16)
	return hal_CHECK_BIT(PINB, (pin - 8));
	else if (pin < 24)
	return hal_CHECK_BIT(PINC, (pin - 16));
	else if (pin < 32)
	return hal_CHECK_BIT(PIND, (pin - 24));
	else if (pin < 40)
	return hal_CHECK_BIT(PINE, (pin - 32));
	else if (pin < 48)
	return hal_CHECK_BIT(PINF, (pin - 40));
	else if (pin < 53)
	return hal_CHECK_BIT(PING, (pin - 48));
	else {
		return 0xFF;
	}
}