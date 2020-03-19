/*
 * boot.h
 *
 * Created: 05.03.2020 19:49:11
 *  Author: Admin
 */ 


#ifndef BOOT_H_
#define BOOT_H_

#ifndef F_CPU
#define F_CPU 8000000L						//CPU frequency
#endif

#define BOOT_VER "0.0.0-bleeding"
#define BOOT_TIMESTAMP __TIMESTAMP__

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <types.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

void boot_logMessage(uint8_t level, const char * format, ...);
void boot_logMessage_p(uint8_t level, uint_farptr_t format_p, ...);
void boot_checkSignature(uint8_t s0, uint8_t s1, uint8_t s2);
void boot_programPage(uint32_t page, uint8_t *buf);
void boot_runStateMachine();

uint8_t boot_convertAsciiToNumeric(unsigned char c);

#endif /* BOOT_H_ */