/*
 * control.h
 *
 * Created: 14.02.2020 20:53:41
 *  Author: Admin
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/types.h>
#include <kernel/hal/hal.h>

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
#define KFLAG_RROBIN_SWITCHED 6
#define KFLAG_LOG_SD 13
#define KFLAG_LOG_UART 14
#define KFLAG_DEBUG 15

#define KOSSTATUS_INIT 0
#define KOSSTATUS_RUNNING 1
#define KOSSTATUS_HALTED 2
#define KOSSTATUS_ERRORED 3

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);
uint64_t kernel_getUptime();
void kernel_checkMCUCSR();

uint8_t kernel_getSystemStatus();
void kernel_setSystemStatus(uint8_t status);

#endif /* CONTROL_H_ */