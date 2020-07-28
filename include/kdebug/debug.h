/*
 * debug.h
 *
 * Created: 14.02.2020 20:50:18
 *  Author: Admin
 */


#ifndef DEBUG_H_
#define DEBUG_H_

#define DBG_MOD_VER "0.7.2-bleeding"
#define DBG_MOD_TIMESTAMP __TIMESTAMP__

#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>

#define PGM_ON 1
#define PGM_OFF 0
#define PGM_PUTS 2
#define L_NONE 0
#define L_INFO 1
#define L_WARN 2
#define L_ERROR 3
#define L_FATAL 4

void debug_puts(uint8_t level, const char * message); //Only for PROGMEM strings
void debug_logMessage(uint8_t pgm, uint8_t level, const char * format, ...);
void debug_init();

#endif /* DEBUG_H_ */