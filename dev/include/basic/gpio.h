/*
 * gpio.h
 *
 * Created: 13.02.2020 19:39:45
 *  Author: Admin
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define HIGH 1
#define LOW 0
#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

void hal_switchBit(volatile uint8_t *reg, uint8_t bit);
void hal_setBit(volatile uint8_t *reg, uint8_t bit);
void hal_clearBit(volatile uint8_t *reg, uint8_t bit);
uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit);
void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value);
uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin);
void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value);

void gpio_pinMode(uint8_t pin, uint8_t value);
void gpio_digitalWrite(uint8_t pin, uint8_t value);
uint8_t gpio_digitalRead(uint8_t pin);

void hal_setupPins();

#endif /* GPIO_H_ */