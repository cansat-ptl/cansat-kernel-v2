/*
 * debug.c
 *
 * Created: 02.06.2019 20:53:16
 *  Author: ThePetrovich
 */ 

#include <kernel.h>
#include <hal/hal.h>

#if KERNEL_DEBUG_MODULE == 1

#define DBG_MOD_VER "0.6.0-bleeding"
#define DBG_MOD_TIMESTAMP __TIMESTAMP__

const char log_nolevel[] PROGMEM = "";
const char log_levelinfo[] PROGMEM = "[INFO]";
const char log_levelwarn[] PROGMEM = "[WARN]";
const char log_levelerr[] PROGMEM = "[ERROR]";
const char log_levelfatal[] PROGMEM = "[FATAL]";


const char * const levels[] PROGMEM = {
	log_nolevel,
	log_levelinfo,
	log_levelwarn,
	log_levelerr,
	log_levelfatal
};
#endif

inline void debug_sendMessage(char* buffer, uint8_t level, const char * format, va_list args) 
{
	#if KERNEL_DEBUG_MODULE == 1	
		if (level != 0 && !kernel_checkFlag(KFLAG_INIT)) {
			#if PROFILING == 0
				//sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			#else
				sprintf(buffer, "%ld ", (int32_t)kernel_getUptime());
			#endif
			hal_UART_PUTS(buffer);
		}
		strcpy_P(buffer, (char*)pgm_read_word(&(levels[level])));
		hal_UART_PUTS(buffer);
		vsprintf(buffer, format, args);
		hal_UART_PUTS(buffer);
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
}

inline void debug_sendMessage_p(char* buffer, uint8_t level, const char * format, va_list args) 
{
	#if KERNEL_DEBUG_MODULE == 1		
		if (level != 0 && !kernel_checkFlag(KFLAG_INIT)) {
			#if PROFILING == 0
				//sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			#else
				sprintf(buffer, "%ld ", (int32_t)kernel_getUptime());
			#endif
			hal_UART_PUTS(buffer);
		}
		strcpy_P(buffer, (char*)pgm_read_word(&(levels[level])));
		hal_UART_PUTS(buffer);
		vsprintf_P(buffer, format, args);
		hal_UART_PUTS(buffer);
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
}

void debug_sendMessageSD(char* buffer, uint8_t level, const char * format, va_list args)
{
	#if KERNEL_DEBUG_MODULE == 1	
		if (level != 0 && !kernel_checkFlag(KFLAG_INIT)) {
			//sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			sd_puts(buffer);
		}
		strcpy_P(buffer, (char*)pgm_read_word(&(levels[level])));
		sd_puts(buffer);
		vsprintf(buffer, format, args);
		sd_puts(buffer);
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
}

void debug_sendMessageSD_p(char* buffer, uint8_t level, const char * format, va_list args)
{
	#if KERNEL_DEBUG_MODULE == 1
		if (level != 0 && !kernel_checkFlag(KFLAG_INIT)) {
			//sprintf(buffer, "%02d.%02d.%02d %02d:%02d:%02d ", GPS.day, GPS.month, GPS.year, GPS.hour, GPS.minute, GPS.second);
			sd_puts(buffer);
		}
		strcpy_P(buffer, (char*)pgm_read_word(&(levels[level])));
		sd_puts(buffer);
		vsprintf_P(buffer, format, args);
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
}

void debug_puts(uint8_t level, const char * format)
{
	kernel_enterCriticalSection();
	#if KERNEL_DEBUG_MODULE == 1
	char * levelptr = (char*)pgm_read_word(&(levels[level]));
		while(pgm_read_byte(levelptr) != 0x00)
			hal_UART_PUTC(pgm_read_byte(levelptr++));
		while(pgm_read_byte(format) != 0x00)
			hal_UART_PUTC(pgm_read_byte(format++));
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
	kernel_exitCriticalSection();
}

void debug_putsSD(uint8_t level, const char * format)
{
	#if KERNEL_DEBUG_MODULE == 1
	char * levelptr = (char*)pgm_read_word(&(levels[level]));
		while(pgm_read_byte(levelptr) != 0x00)
			sd_putc(pgm_read_byte(levelptr++));
		while(pgm_read_byte(format) != 0x00)
			sd_putc(pgm_read_byte(format++));
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
}

static char __buffer[128];

void debug_logMessage(uint8_t pgm, uint8_t level, const char * format, ...)
{
	kernel_enterCriticalSection();
	#if KERNEL_DEBUG_MODULE == 1
		va_list args;
		
		va_start(args, format);
		switch(pgm){
			case 0:
				if(__buffer == NULL) return;
				
				debug_sendMessage(__buffer, level, format, args);
				#if KERNEL_SD_MODULE == 1
					debug_sendMessageSD(__buffer, level, format, args);
				#endif
			break;
			case 1:
				if(__buffer == NULL) return;
				
				debug_sendMessage_p(__buffer, level, format, args);
				#if KERNEL_SD_MODULE == 1
					debug_sendMessageSD_p(__buffer, level, format, args);
				#endif
				
			break;
			case 2:
				debug_puts(level, format);
				#if KERNEL_SD_MODULE == 1
					debug_putsSD(level, format);
				#endif
			break;
		}
		va_end(args);
	#else
		#warning Trying to use disabled debug module, this may spawn dragons
	#endif
	kernel_exitCriticalSection();
}
