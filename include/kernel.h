/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 

#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.0.0-bleeding"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#include <types.h>
#include <hal/hal.h>
#include <threads/threads.h>
#include "../kernel_config.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ERR_KRN_STACK_OVERFLOW 1
#define ERR_NULLPTR 2
#define ERR_MEMORY_CORRUPTION 3
#define ERR_QUEUE_END 4
#define ERR_WDT_RESET 5
#define ERR_BOD_RESET 6
#define ERR_KRN_RETURN 7
#define ERR_DEVICE_FAIL 8
#define ERR_GENERIC 255

#define KFLAG_INIT 0
#define KFLAG_TIMER_SET 1
#define KFLAG_TIMER_EN 2
#define KFLAG_TIMER_ISR 3
#define KFLAG_SD_INIT 4
#define KFLAG_CSW_ALLOWED 5
#define KFLAG_LOG_SD 13
#define KFLAG_LOG_UART 14
#define KFLAG_DEBUG 15

void kernel_exitCriticalSection();
void kernel_enterCriticalSection();

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);
uint64_t kernel_getUptime();
kTaskHandle_t kernel_createTask(kTask_t t_pointer, kStackSize_t t_stackSize, kTaskPriority_t t_priority, kTaskType_t t_type);
uint8_t kernel_setTaskState(kTaskHandle_t t_handle, kTaskStatus_t t_state);

void kernel_sortTaskList(kTaskHandle_t taskList, uint8_t amount);
kStackPtr_t kernel_setupTaskStack(kTask_t startupPointer, kStackSize_t taskStackSize, kTaskType_t taskType);
void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler);

void kernel_yield() __attribute__ (( naked, noinline ));

kTaskHandle_t kernel_getCurrentTaskHandle();
kTaskHandle_t kernel_getNextTaskHandle();
kTaskHandle_t kernel_getTaskListPtr();
kStackPtr_t kernel_getStackPtr();
kStackSize_t kernel_getUserTaskStackUsage();
kStackSize_t kernel_getSystemTaskStackUsage();
uint8_t kernel_getTaskListIndex();
void kernel_setCurrentTask(kTaskHandle_t taskHandle);

void kernel_checkMCUCSR();
uint8_t kernel_init();

uint8_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_period);
uint8_t kernel_removeTimer(kTimerISR_t t_pointer);
void kernel_timerService();

#ifdef KERNEL_SD_MODULE
	void sd_puts(char * data);
	void sd_flush();
	void sd_readPtr();
	void sd_init();
#endif

#ifdef KERNEL_WDT_MODULE
	void wdt_saveMCUCSR(void) __attribute__((naked)) __attribute__((section(".init3")));
	//void wdt_disableWatchdog();
	void wdt_enableWatchdog();
#endif

#ifdef KERNEL_UTIL_MODULE
	#define util_GET_ARRAY_LENGTH(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))
	void util_printVersion();
	uint8_t util_strCompare(char * a, char * b, uint8_t len);
#endif
	
#ifdef KERNEL_DEBUG_MODULE
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

#endif /* KERNEL_H_ */