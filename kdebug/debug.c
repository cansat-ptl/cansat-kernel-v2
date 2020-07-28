/*
 * debug.c
 *
 * Created: 02.06.2019 20:53:16
 *  Author: ThePetrovich
 */

#include "hal.h"
#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/ktypes.h>
#include <kernel/utils.h>
#include <kernel/kernel_config.h>

//TODO: remove stdio

#define debug_GET_STATUS() kernel_getSystemStatus()

#if CFG_DEBUG_PRINT_LEVELS == 1
const char log_nolevel[] PROGMEM = "";
const char log_levelinfo[] PROGMEM = "[INFO] ";
const char log_levelwarn[] PROGMEM = "[WARN] ";
const char log_levelerr[] PROGMEM = "[ERROR] ";
const char log_levelfatal[] PROGMEM = "[FATAL] ";

const char * const levels[] PROGMEM = {
	log_nolevel,
	log_levelinfo,
	log_levelwarn,
	log_levelerr,
	log_levelfatal
};
#endif

#if CFG_DEBUG_PRINT_STAGES == 1
const char log_stageInit[] PROGMEM = "<INIT>";
const char log_stageRun[] PROGMEM = "<EXEC>";
const char log_stageHalt[] PROGMEM = "<HALT>";
const char log_stageErr[] PROGMEM = "<ERRD>";

const char * const stages[] PROGMEM = {
	log_stageInit,
	log_stageRun,
	log_stageHalt,
	log_stageErr,
};
#endif

#if CFG_DEBUG_PRINT_LEVELS == 1
void debug_printLevel(uint8_t level)
{
	char * levelptr = (char*)hal_READ_WORD_FROM_FLASH(&(levels[level])); //-V2573

	while(hal_READ_BYTE_FROM_FLASH(levelptr) != 0x00) {//-V2573
		hal_UART_PUTC(hal_READ_BYTE_FROM_FLASH(levelptr++)); //-V2573
    }
}
#endif

#if CFG_DEBUG_PRINT_STAGES == 1
void debug_printStage()
{
	uint8_t stage = debug_GET_STATUS();
	char * stageptr = (char*)hal_READ_WORD_FROM_FLASH(&(stages[stage])); //-V2573 //-V2571

	while(hal_READ_BYTE_FROM_FLASH(stageptr) != 0x00) { //-V2573 //-V2571
		hal_UART_PUTC(hal_READ_BYTE_FROM_FLASH(stageptr++)); //-V2573 //-V2571
    }
}
#endif

inline void debug_sendMessage(uint8_t level, const char * format, va_list args)
{
	if (level != 0u) {
		#if CFG_PROFILING == 0
			time_updateSystemTime();
			printf_P(PSTR("[%02d:%02d:%02d.%03d]"), time_getHours(), time_getMinutes(), time_getSeconds(), time_getMilliseconds()); //-V2573
		#else
			printf_P(PSTR("[%8ld]"), (int32_t)kernel_getUptime());
		#endif

		#if CFG_DEBUG_PRINT_STAGES == 1
			debug_printStage();
		#endif
		#if CFG_DEBUG_PRINT_LEVELS == 1
			debug_printLevel(level);
		#endif
	}

	vfprintf(stdout, format, args);
}

inline void debug_sendMessage_p(uint8_t level, const char * format, va_list args)
{
	if (level != 0u) {
		#if CFG_PROFILING == 0
			time_updateSystemTime();
			printf_P(PSTR("[%02d:%02d:%02d.%03d]"), time_getHours(), time_getMinutes(), time_getSeconds(), time_getMilliseconds()); //-V2573
		#else
			printf_P(PSTR("[%8ld]"), (int32_t)kernel_getUptime());
		#endif

		#if CFG_DEBUG_PRINT_STAGES == 1
			debug_printStage();
		#endif
		#if CFG_DEBUG_PRINT_LEVELS == 1
			debug_printLevel(level);
		#endif
	}

	vfprintf_P(stdout, format, args);
}

void debug_puts(uint8_t level, const char * format)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (level != 0u) {

		#if CFG_PROFILING == 0
			time_updateSystemTime();
			printf_P(PSTR("[%02d:%02d:%02d.%03d]"), time_getHours(), time_getMinutes(), time_getSeconds(), time_getMilliseconds()); //-V2573
		#else
			printf_P(PSTR("[%8ld]"), (int32_t)kernel_getUptime());
		#endif

		#if CFG_DEBUG_PRINT_STAGES == 1
			debug_printStage();
		#endif
		#if CFG_DEBUG_PRINT_LEVELS == 1
			debug_printLevel(level);
		#endif
	}

	while(hal_READ_BYTE_FROM_FLASH(format) != 0x00) { //-V2573 //-V2571
		hal_UART_PUTC(hal_READ_BYTE_FROM_FLASH(format++)); //-V2573 //-V2571
    }

	threads_endAtomicOperation(sreg);
}

void debug_logMessage(uint8_t pgm, uint8_t level, const char * format, ...)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	va_list args;

	va_start(args, format);
	switch(pgm){
		case 0:
			debug_sendMessage(level, format, args);
		break;
		case 1:
			debug_sendMessage_p(level, format, args);
		break;
		case 2:
			debug_puts(level, format);
		break;
        default:
            //Unknown PGM parameter value, do nothing
        break;
	}
	va_end(args);
	threads_endAtomicOperation(sreg);
}
