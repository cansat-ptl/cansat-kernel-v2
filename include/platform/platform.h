/*
 * platform.h
 *
 * Created: 15.02.2020 19:30:29
 *  Author: Admin
 */ 


#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <platform/avr/avr.h>
#include <platform/avr/timers.h>

uint8_t platform_prepareStackFrame(kStackPtr_t regionPointer, kTask_t taskPointer);

#endif /* PLATFORM_H_ */