/*
 * hal_pins.h
 *
 * Created: 06.12.2019 14:53:02
 *  Author: ThePetrovich
 */ 


#ifndef HAL_PINS_H_
#define HAL_PINS_H_

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

#endif /* HAL-PINS_H_ */