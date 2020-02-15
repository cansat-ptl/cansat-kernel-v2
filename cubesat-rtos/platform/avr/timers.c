/*
 * timers.c
 *
 * Created: 13.02.2020 19:34:12
 *  Author: Admin
 */ 
#include <platform/platform.h>
#include <kernel.h>

void platform_setupTimer1A(uint8_t prescaler)
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TCCR1B |= (1 << WGM12)|(prescaler << CS10); // prescaler 64 cs11 & cs10 = 1
	TCNT1 = 0;
	OCR1A = 125;
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_startTimer1A()
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE1A);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_stopTimer1A()
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE1A);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_setupTimer0(uint8_t prescaler)
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TCCR0 |= (prescaler << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = 125;
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_startTimer0()
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE0);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_stopTimer0()
{
	uint8_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE0);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

ISR(TIMER0_COMP_vect, ISR_NAKED) 
{
	kernel_tick();
	platform_RETI();
}