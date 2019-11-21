/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */ 

#ifndef HAL_H_
#define HAL_H_

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define HAL_MOD_VER "0.2.0-staging"
#define HAL_MOD_TIMESTAMP __TIMESTAMP__

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include "kernel_config.h"

#define JUMPER_PORT PORTA
#define JUMPER_DDR DDRA
#define JUMPER_PIN PINA
#define JUMPER_IN PA0
#define JUMPER_OUT PA1

#define LED_BUILTIN_PORT PORTG
#define LED_BUILTIN_DDR DDRG
#define LED_BUILTIN PG3

#define LED_WRK_PORT PORTC
#define LED_WRK_DDR DDRC
#define LED_WRK PC4

#define LED_KRN_PORT PORTC
#define LED_KRN_DDR DDRC
#define LED_KRN PC5

#define LED_TX_PORT PORTC
#define LED_TX_DDR DDRC
#define LED_TX PC6

#define LED_DBG_PORT PORTC
#define LED_DBG_DDR DDRC
#define LED_DBG PC7

#define HIGH 1
#define LOW 0
#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

#ifdef HAL_USE_TIMER0
	#define HAL_TIMER_INTERRUPT_vect TIMER0_COMP_vect
	#define hal_setupSystemTimer() hal_setupTimer0(KERNEL_TIMER_PRESCALER)
	#define hal_startSystemTimer() hal_startTimer0()
	#define hal_stopSystemTimer() hal_stopTimer0()
#endif
#ifdef HAL_USE_TIMER1A
	#define HAL_TIMER_INTERRUPT_vect TIMER1_COMPA_vect
	#define hal_setupSystemTimer() hal_setupTimer1A(KERNEL_TIMER_PRESCALER)
	#define hal_startSystemTimer() hal_startTimer1A()
	#define hal_stopSystemTimer() hal_stopTimer1A()
#endif

#define hal_disableInterrupts() cli()
#define hal_enableInterrupts() sei()

#define hal_statusReg SREG
#define hal_nop() asm volatile ("NOP")
#define delay_ms(x) _delay_ms(x);

#define hal_setBit_m(x,y) x |= (1 << y)
#define hal_clearBit_m(x,y) x &= ~(1 << y)
#define hal_checkBit_m(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define hal_switchBit_m(x,y) (x ^= (1 << y))
#define hal_writeBit_m(x,y,z) x ^= (-1 * z ^ x) & (1 << y);

inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_setBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit);
inline volatile uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value);
inline volatile uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin);
inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value);

void hal_pinMode(uint8_t pin, uint8_t value);
void hal_digitalWrite(uint8_t pin, uint8_t value);
uint8_t hal_digitalRead(uint8_t pin);

void hal_stopTimer1A();
void hal_startTimer1A();
void hal_setupTimer1A(uint8_t prescaler);
void hal_stopTimer0();
void hal_startTimer0();
void hal_setupTimer0(uint8_t prescaler);

#ifndef hal_uart_init
#define hal_uart_init(ubrr) hal_basicUart_init(ubrr)
int hal_basicUart_init(unsigned int ubrr);
#endif

#ifndef hal_uart_putc
#define hal_uart_putc(c) hal_basicUart_putc(c)
void hal_basicUart_putc(char c);
#endif

#ifndef hal_uart_puts
#define hal_uart_puts(s) hal_basicUart_puts(s)
void hal_basicUart_puts(char * msg);
#endif

#ifndef hal_uart_enableInterruptsRX
#define hal_uart_enableInterruptsRX() hal_basicUart_enableInterruptsRX()
void hal_basicUart_enableInterruptsRX();
#endif

#ifndef hal_uart_disableInterruptsRX
#define hal_uart_disableInterruptsRX(s) hal_basicUart_disableInterruptsRX()
void hal_basicUart_disableInterruptsRX();
#endif


inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void hal_setBit(volatile uint8_t *reg, uint8_t bit){
	*reg |= (1 << bit);
}

inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit){
	*reg &= ~(1 << bit);
}

inline volatile uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit){
	return (*reg >> bit) & 1;
}

inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*port ^= (-1 * nvalue ^ *port) & (1 << pin);
}
//Actually does the same thing as checkBit(), but i'll keep it here just in case
inline volatile uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin){
	return (*port >> pin) & 1;
}

inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value){
	uint8_t nvalue = !value;
	*ddr ^= (-1 * nvalue ^ *ddr) & (1 << pin);
}

void hal_setupPins();
#endif /* HAL_H_ */