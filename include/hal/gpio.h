/*
 * gpio.h
 *
 * Created: 13.02.2020 19:39:45
 *  Author: Admin
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#define HIGH 1
#define LOW 0
#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_setBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit);
inline uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit);
inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value);
inline uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin);
inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value);

void hal_pinMode(uint8_t pin, uint8_t value);
void hal_digitalWrite(uint8_t pin, uint8_t value);
uint8_t hal_digitalRead(uint8_t pin);

inline void hal_switchBit(volatile uint8_t *reg, uint8_t bit){
	*reg ^= (1 << bit);
}

inline void hal_setBit(volatile uint8_t *reg, uint8_t bit){
	*reg |= (1 << bit);
}

inline void hal_clearBit(volatile uint8_t *reg, uint8_t bit){
	*reg &= ~(1 << bit);
}

inline uint8_t hal_checkBit(volatile uint8_t *reg, uint8_t bit){
	return (*reg >> bit) & 1;
}

inline void hal_writePin(volatile uint8_t *port, uint8_t pin, uint8_t value){
	uint8_t nvalue = !!value;
	*port ^= (-1 * nvalue ^ *port) & (1 << pin);
}
//Actually does the same thing as checkBit(), but i'll keep it here just in case
inline uint8_t hal_readPin(volatile uint8_t *port, uint8_t pin){
	return (*port >> pin) & 1;
}

inline void hal_setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t value){
	uint8_t nvalue = !value;
	*ddr ^= (-1 * nvalue ^ *ddr) & (1 << pin);
}

void hal_setupPins();

#endif /* GPIO_H_ */