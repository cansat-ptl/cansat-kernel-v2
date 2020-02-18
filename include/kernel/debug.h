/*
 * debug.h
 *
 * Created: 14.02.2020 20:50:18
 *  Author: Admin
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <types.h>
#include <hal/hal.h>

#ifdef CFG_KERNEL_DEBUG_MODULE
	#define PGM_ON 1
	#define PGM_OFF 0
	#define PGM_PUTS 2
	#define L_NONE 0
	#define L_INFO 1
	#define L_WARN 2
	#define L_ERROR 3
	#define L_FATAL 4
	void debug_puts(uint8_t level, const char * message); //Only for PROGMEM strings
	void debug_putsSD(uint8_t level, const char * message);
	void debug_logMessage(uint8_t pgm, uint8_t level, const char * format, ...);
#endif

#ifdef KERNEL_SD_MODULE
	void sd_puts(char * data);
	void sd_flush();
	void sd_readPtr();
	void sd_init();
#endif



#endif /* DEBUG_H_ */