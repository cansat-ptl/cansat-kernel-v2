/*
 * timers.c
 *
 * Created: 13.02.2020 19:34:12
 *  Author: Admin
 */
#include <kernel/platform/platform.h>
#include <kernel/kernel.h>

void platform_setupTimer1A(uint8_t prescaler)
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TCCR1B |= (1 << WGM12)|(prescaler << CS10); // prescaler 64 cs11 & cs10 = 1
	TCNT1 = 0;
	OCR1A = CFG_TIMER_COMPARE_VALUE;
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_startTimer1A()
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE1A);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_stopTimer1A()
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE1A);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_setupTimer0(uint8_t prescaler)
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TCCR0 |= (prescaler << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = CFG_TIMER_COMPARE_VALUE; //Corrected accordingly to ISR execution time
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_startTimer0()
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE0);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

void platform_stopTimer0()
{
	kStatusRegister_t sreg = platform_STATUS_REG;
	platform_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE0);
	platform_ENABLE_INTERRUPTS();
	platform_STATUS_REG = sreg;
}

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
	platform_tick();
	platform_RETI();
}

