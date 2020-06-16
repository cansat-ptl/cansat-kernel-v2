/*
 * softtimers.h
 *
 * Created: 16.06.2020 16:46:13
 *  Author: Admin
 */ 


#ifndef SOFTTIMERS_H_
#define SOFTTIMERS_H_

typedef void (*kTimerISR_t)(void);

struct kTimerStruct_t
{
	kTimerISR_t tsrPointer;
	uint32_t period;
	uint32_t repeatPeriod;
};

#endif /* SOFTTIMERS_H_ */