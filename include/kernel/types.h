/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef uint32_t size_t;
#endif

#include <stddef.h>

typedef void (*kTask_t)(void*);
typedef void (*kTimerISR_t)(void);
typedef volatile uint8_t *kStackPtr_t;
typedef uint16_t kStackSize_t;
typedef int16_t kIterator_t;
typedef uint8_t kRegister_t;
typedef uint8_t kStatusRegister_t;
typedef uint32_t kPointerValue_t;
typedef void kTask;

typedef uint8_t byte;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_SLEEPING, KSTATE_BLOCKED, KSTATE_SEMAPHORE, KSTATE_READY, KSTATE_RUNNING} kTaskState_t;
typedef enum {KEVENT_NONE, KEVENT_FIRED} kEventState_t;
typedef enum {KTASK_USER, KTASK_SYSTEM} kTaskType_t;
typedef enum {KTIMER_SINGLERUN, KTASK_REPEATED} kTimerType_t;
typedef enum {KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;

typedef volatile struct kTaskStruct_t* kTaskHandle_t;
typedef volatile struct kTimerStruct_t* kTimerHandle_t;

typedef struct kLock_t kMutex_t;
typedef struct kLock_t kSemaphore_t;
typedef uint8_t kSpinlock_t;

typedef kMutex_t* kMutexHandle_t;
typedef kSemaphore_t* kSemaphoreHandle_t;
typedef kSpinlock_t* kSpinlockHandle_t;

typedef struct kSystemIO_t kLifo_t;
typedef struct kSystemIO_t kFifo_t;

typedef struct kSystemIO_t* kLifoHandle_t;
typedef struct kSystemIO_t* kFifoHandle_t;
typedef struct kSystemIO_t* kSystemIOHandle_t;


struct kLock_t
{
	kTaskHandle_t owner;
	uint8_t lockCount;
	kLockType_t type;
};

struct kSystemIO_t
{
	char* pointer;
	size_t itemSize;
	size_t size;
	uint16_t inputPosition;
	uint16_t outputPosition;
	uint16_t currentPosition;
};

struct kEvent_t
{
	kEventState_t state;
	uint16_t eventFlags;
};

struct kTaskStruct_t
{
	kStackPtr_t stackPtr;
	kStackPtr_t stackBegin;
	kTask_t taskPtr;
	void* args;
	kStackSize_t stackSize;
	uint8_t priority;
	kTaskState_t state;
	uint16_t sleepTime;
	struct kLock_t* lock;
	struct kEvent_t notification;
	kTaskType_t type;
	uint8_t pid;
	uint8_t flags;
	char* name;
};

struct kMemoryBlock_t
{
	struct kMemoryBlock_t* next;
	size_t blockSize;
	uint8_t state;
};

struct kTimerStruct_t
{
	kTimerISR_t tsrPointer;
	uint32_t period;
	uint32_t repeatPeriod;
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