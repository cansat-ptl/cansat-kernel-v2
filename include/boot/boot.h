/*
 * boot.h
 *
 * Created: 05.03.2020 19:49:11
 *  Author: Admin
 */ 


#ifndef BOOT_H_
#define BOOT_H_

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
void boot_logMessage_p(uint8_t level, const char * format, ...);

#endif /* BOOT_H_ */