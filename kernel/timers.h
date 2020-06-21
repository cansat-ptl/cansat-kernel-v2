/*
 * timers.h
 *
 * Created: 21.06.2020 20:34:59
 *  Author: Admin
 */


#ifndef TIMERS_H_
#define TIMERS_H_

struct kTimerStruct_t
{
	kTimerISR_t tsrPointer;
	uint32_t period;
	uint32_t repeatPeriod;
};

#endif /* TIMERS_H_ */