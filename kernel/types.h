/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */ 

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef void (*kTask_t)(void);
typedef void (*kTimerISR_t)(void);
typedef volatile uint8_t *kStackPtr_t;
typedef uint8_t byte;
typedef void kTask;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_SEMAPHORE, KSTATE_READY, KSTATE_RUNNING} kTaskStatus_t;
typedef enum {KTASK_USER, KTASK_SYSTEM} kTaskType_t;
typedef enum {KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;
typedef enum {KPRIO_HIGH, KPRIO_NORM, KPRIO_LOW, KPRIO_NONE} kTaskPriority_t;
typedef volatile struct kTaskStruct_t* kTaskHandle_t;
typedef struct kLock_t kMutex_t;

struct kLock_t 
{
	uint8_t locks;
	kLockType_t type;
};

struct kTaskStruct_t 
{
	kStackPtr_t stackPtr;
	kTask_t taskPtr;
	kStackPtr_t stackBegin;
	uint16_t stackSize;
	kTaskPriority_t priority;
	kTaskStatus_t status;
	struct kLock_t* lock;
	kTaskType_t type;
	uint16_t execTime;
	uint16_t execTimeMax;
	uint8_t pid;
};

struct kTimerStruct_t 
{
	kTimerISR_t tsrPointer;
	uint32_t period;
	uint32_t savePeriod;
};

struct kSystemTime_t 
{
	uint16_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};


#endif /* TYPES_H_ */