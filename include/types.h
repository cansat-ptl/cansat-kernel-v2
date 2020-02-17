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
typedef uint16_t kStackSize_t;
typedef void kTask;

typedef uint8_t byte;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_SLEEPING, KSTATE_BLOCKED, KSTATE_SEMAPHORE, KSTATE_READY, KSTATE_RUNNING} kTaskState_t;
typedef enum {KTASK_USER, KTASK_SYSTEM} kTaskType_t;
typedef enum {KTIMER_SINGLERUN, KTASK_REPEATED} kTimerType_t;
typedef enum {KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;
typedef volatile struct kTaskStruct_t* kTaskHandle_t;
typedef struct kLock_t kMutex_t;
typedef struct kLock_t kSemaphore_t;
typedef uint8_t kSpinlock_t;

struct kLock_t 
{
	kTaskHandle_t owner;
	uint8_t lockCount;
	kLockType_t type;
};

struct kTaskStruct_t 
{
	kStackPtr_t stackPtr;
	kTask_t taskPtr;
	kStackPtr_t stackBegin;
	kStackSize_t stackSize;
	uint8_t priority;
	kTaskState_t state;
	uint16_t sleepTime;
	struct kLock_t* lock;
	kTaskType_t type;
	uint8_t position;
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