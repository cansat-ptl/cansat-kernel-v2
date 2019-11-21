/*
 * kernel_utils.c
 *
 * Created: 15.09.2019 17:03:39
 *  Author: Admin
 */ 

#include "../kernel.h"

void kernel_checkMCUCSR()
{
	/*
	if(hal_checkBit_m(mcucsr_mirror, WDRF)){
		kernel_handleError(ERR_WDT_RESET);
		hal_setBit_m(kflags, WDRF);
		return;
	}
	if(hal_checkBit_m(mcucsr_mirror, BORF)){
		kernel_handleError(ERR_BOD_RESET);
		hal_setBit_m(kflags, BORF);
	}
	return;*/
}

static void kernel_handleError(uint8_t error)
{
	if(kernel_checkFlag(KFLAG_DEBUG)){
		switch(error){
			case ERR_WDT_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] The system has been reset by watchdog.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] This is usually caused by software issues or faulty device connections.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, report this to the developer as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: MCUCSR.WDRF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
			break;
			case ERR_BOD_RESET:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] The system has been reset by brown-out detector.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] This is usually caused by an unstable power supply.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, check power supply wire connections and circuitry as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: MCUCSR.BORF = 1\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
			break;
			case ERR_DEVICE_FAIL:
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("\r\n--------------------------------------------------------------------------------\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] A major device failure has been reported by one of the tasks.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] To prevent damage and data corruption, the task has been suspended.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Please, check your device connections and circuitry as soon as possible.\r\n"));
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("[FATAL] Error details: taskReturnCode = ERR_DEVICE_FAIL\r\n")); //TODO: implement verbose error info, e.g what device has failed
			debug_logMessage(PGM_ON, L_NONE, (char *)PSTR("--------------------------------------------------------------------------------\r\n\r\n"));
			break;
		}
	}
}