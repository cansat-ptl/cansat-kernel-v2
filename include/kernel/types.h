/*
 * types.h
 *
 * Created: 12.05.2019 18:36:04
 *  Author: ThePetrovich
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <kernel/platform/types.h>

#define ERR_GENERIC -1
#define ERR_NULLPTR -2

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

typedef void kTask;
typedef void (*kTask_t)(void*);
typedef void (*kTimerISR_t)(void);

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_SLEEPING, KSTATE_BLOCKED, KSTATE_READY, KSTATE_RUNNING} kTaskState_t;
typedef enum {KEVENT_NONE, KEVENT_FIRED} kEventState_t;
typedef enum {KTASK_USER, KTASK_SYSTEM} kTaskType_t;
typedef enum {KTIMER_SINGLERUN, KTASK_REPEATED} kTimerType_t;
typedef enum {KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;

typedef volatile struct kTaskStruct_t* kTaskHandle_t;
typedef volatile struct kTimerStruct_t* kTimerHandle_t;

typedef volatile struct kLockStruct_t kMutex_t;
typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile uint8_t kSpinlock_t;

typedef volatile kMutex_t* kMutexHandle_t;
typedef volatile kSemaphore_t* kSemaphoreHandle_t;
typedef volatile kSpinlock_t* kSpinlockHandle_t;

typedef struct kIPCStruct_t kLifo_t;
typedef struct kIPCStruct_t kFifo_t;

typedef struct kIPCStruct_t* kLifoHandle_t;
typedef struct kIPCStruct_t* kFifoHandle_t;
typedef struct kIPCStruct_t* kSystemIOHandle_t;

struct kLinkedListStruct_t
{
	kTaskHandle_t head;
	kTaskHandle_t tail;
};

struct kListItemStruct_t
{
	volatile struct kLinkedListStruct_t* list;
	kTaskHandle_t next;
	kTaskHandle_t prev;
};

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

struct kIPCStruct_t
{
	char* pointer;
	size_t itemSize;
	size_t size;
	size_t inputPosition;
	size_t outputPosition;
	size_t currentPosition;
};

struct kEventStruct_t
{
	kEventState_t state;
	uint16_t eventFlags;
};

struct kTaskStruct_t
{
	kStackPtr_t stackPtr;
	kTask_t taskPtr;
	void* args;
	kStackPtr_t stackBegin;
	kStackSize_t stackSize;
	volatile struct kLockStruct_t* lock;
	volatile struct kEventStruct_t notification;
	kTaskState_t state;
	kTaskTicks_t sleepTime;
	kTaskType_t type;
	uint8_t priority;
	uint8_t flags;
	kPid_t pid;
	char* name;
	struct kListItemStruct_t taskList;
};

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	uint8_t state;
};

struct kTimerStruct_t
{
	kTimerISR_t tsrPointer;
	uint32_t period;
	uint32_t repeatPeriod;
};

struct kSystemTimeStruct_t
{
	uint16_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};

#endif /* TYPES_H_ */