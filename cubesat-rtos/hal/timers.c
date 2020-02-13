/*
 * timers.c
 *
 * Created: 13.02.2020 19:34:12
 *  Author: Admin
 */ 
#include <hal/hal.h>

void hal_setupTimer1A(uint8_t prescaler)
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TCCR1B |= (1 << WGM12)|(prescaler << CS10); // prescaler 64 cs11 & cs10 = 1
	TCNT1 = 0;
	OCR1A = 125;
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}

void hal_startTimer1A()
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE1A);
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}

void hal_stopTimer1A()
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE1A);
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}

void hal_setupTimer0(uint8_t prescaler)
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TCCR0 |= (prescaler << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = 125;
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}

void hal_startTimer0()
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE0);
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}

void hal_stopTimer0()
{
	uint8_t sreg = hal_STATUS_REG;
	hal_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE0);
	hal_ENABLE_INTERRUPTS();
	hal_STATUS_REG = sreg;
}