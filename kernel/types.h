/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */ 

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef void (*kTask)(void);
typedef void (*kTimerISR)(void);
typedef uint8_t byte;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_SEMAPHORE, KSTATE_READY, KSTATE_RUNNING} kTaskStatus_t;
typedef enum {KTASK_DEFAULT} kTaskType_t;
typedef enum {KPRIO_HIGH, KPRIO_NORM, KPRIO_LOW, KPRIO_NONE} kTaskPriority_t;

struct kTaskStruct_t {
	void *stackPtr;
	kTask taskPtr;
	uint16_t stackSize;
	kTaskPriority_t priority;
	kTaskStatus_t status;
	kTaskType_t type;
	uint16_t execTime;
	uint16_t execTimeMax;
	uint8_t pid;
	char name[8];
};

struct kTimerStruct_t {
	kTimerISR tsrPointer;
	uint32_t period;
	uint32_t savePeriod;
};



#endif /* TYPES_H_ */