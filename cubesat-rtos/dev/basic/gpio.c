/*
 * io.c
 *
 * Created: 13.02.2020 19:32:11
 *  Author: Admin
 */
#include <dev/basic/gpio.h>
#include <kernel/hal/hal.h>

//0 - OUTPUT, 1 - INPUT, 2 - INPUT_PULLUP
void gpio_pinMode(uint8_t pin, uint8_t value)
{
	uint8_t nvalue = !value; //-V2544 //-V2570 //-V2572
	if (pin < 8u) {
		hal_WRITE_BIT(DDRA, pin, nvalue); //-V2568 //-V2544 //-V2571 //-V2572
	}
	else {
		  if (pin < 16u) {
			hal_WRITE_BIT(DDRB, (uint8_t)(pin - 8u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
		}
		else {
			if (pin < 24u) {
				hal_WRITE_BIT(DDRC, (uint8_t)(pin - 16u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
			}
			else {
				if (pin < 32u) {
					hal_WRITE_BIT(DDRD, (uint8_t)(pin - 24u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
				}
				else {
					if (pin < 40u) {
						hal_WRITE_BIT(DDRE, (uint8_t)(pin - 32u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
					}
					else {
						if (pin < 48u) {
							hal_WRITE_BIT(DDRF, (uint8_t)(pin - 40u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
						}
						else {
							if (pin < 53u) {
								hal_WRITE_BIT(DDRG, (uint8_t)(pin - 48u), nvalue); //-V2568 //-V2544 //-V2571 //-V2572
							}
							else {
								//Unexpected PIN value, do nothing
							}
						}
					}
				}
			}
		}
	}
	if(value == INPUT_PULLUP) //-V2568 //-V2507
	gpio_digitalWrite(pin, HIGH);
	return;
}

void gpio_digitalWrite(uint8_t pin, uint8_t value)
{
	if (pin < 8u) {
		hal_WRITE_BIT(PORTA, (uint8_t)pin, value); //-V2568 //-V2544 //-V2571
	}
	else {
		if (pin < 16u) {
			hal_WRITE_BIT(PORTB, (uint8_t)(pin - 8u), value); //-V2568 //-V2544 //-V2571
		}
		else {
			if (pin < 24u) {
				hal_WRITE_BIT(PORTC, (uint8_t)(pin - 16u), value); //-V2568 //-V2544 //-V2571
			}
			else {
				if (pin < 32u) {
					hal_WRITE_BIT(PORTD, (uint8_t)(pin - 24u), value); //-V2568 //-V2544 //-V2571
				}
				else {
					if (pin < 40u) {
						hal_WRITE_BIT(PORTE, (uint8_t)(pin - 32u), value); //-V2568 //-V2544 //-V2571
					}
					else {
						if (pin < 48u) {
							hal_WRITE_BIT(PORTF, (uint8_t)(pin - 40u), value); //-V2568 //-V2544 //-V2571
						}
						else {
							if (pin < 53u) {
								hal_WRITE_BIT(PORTG, (uint8_t)(pin - 48u), value); //-V2568 //-V2544 //-V2571
							}
							else {
								//Unexpected PIN value, do nothing
							}
						}
					}
				}
			}
		}
	}
	return;
}

uint8_t gpio_digitalRead(uint8_t pin)
{
	uint8_t res = 0;
	if (pin < 8u) {
		res = hal_CHECK_BIT(PINA, (uint8_t)pin); //-V2571
	}
	else {
		if (pin < 16u) {
			res = hal_CHECK_BIT(PINB, (uint8_t)(pin - 8u)); //-V2571
		}
		else {
			if (pin < 24u) {
				res = hal_CHECK_BIT(PINC, (uint8_t)(pin - 16u)); //-V2571
			}
			else {
				if (pin < 32u) {
					res = hal_CHECK_BIT(PIND, (uint8_t)(pin - 24u)); //-V2571
				}
				else {
					if (pin < 40u) {
						res = hal_CHECK_BIT(PINE, (uint8_t)(pin - 32u)); //-V2571
					}
					else {
						if (pin < 48u) {
							res = hal_CHECK_BIT(PINF, (uint8_t)(pin - 40u)); //-V2571
						}
						else {
							if (pin < 53u) {
								res = hal_CHECK_BIT(PING, (uint8_t)(pin - 48u)); //-V2571
							}
							else {
								res = 0xFFu;
							}
						}
					}
				}
			}
		}
	}
	return res;
}