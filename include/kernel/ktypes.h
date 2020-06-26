/*
 * ktypes.h
 *
 * Created: 21.06.2020 20:11:17
 *  Author: Admin
 */


#ifndef KTYPES_H_
#define KTYPES_H_

#include <stdint.h>
#include <stddef.h>

typedef void kTask;
typedef void (*kTask_t)(void*);
typedef void (*kTimerISR_t)(void);

#ifndef _SIZE_T
	#define _SIZE_T
	typedef uint32_t size_t;
#endif

typedef uint32_t kPointerValue_t;
typedef volatile uint8_t* kStackPtr_t;

typedef int16_t kIterator_t;
typedef int16_t kReturnValue_t;
typedef int16_t kError_t;

typedef uint8_t byte;
typedef uint8_t kRegister_t;
typedef uint8_t kStatusRegister_t;

typedef uint16_t kStackSize_t;
typedef uint16_t kTaskTicks_t;
typedef uint16_t kPid_t;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_SLEEPING, KSTATE_BLOCKED, KSTATE_READY, KSTATE_RUNNING} kTaskState_t;
typedef enum {KEVENT_UNINIT, KEVENT_NONE, KEVENT_FIRED} kEventState_t;
typedef enum {KTASK_UNINIT, KTASK_USER, KTASK_SYSTEM} kTaskType_t; //TODO: rename
typedef enum {KTIMER_UNINIT, KTIMER_SINGLERUN, KTASK_REPEATED} kTimerType_t;
typedef enum {KLOCK_UNINIT, KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;

struct kTaskStruct_t;
struct kTimerStruct_t;
struct kLockStruct_t;
struct kIPCStruct_t;

typedef volatile struct kTaskStruct_t* kTaskHandle_t;
typedef volatile struct kTimerStruct_t* kTimerHandle_t;

typedef volatile uint8_t kSpinlock_t;

typedef volatile kLockStruct_t* kMutexHandle_t;
typedef volatile kLockStruct_t* kSemaphoreHandle_t;
typedef volatile kSpinlock_t* kSpinlockHandle_t;

typedef struct kIPCStruct_t kLifo_t;
typedef struct kIPCStruct_t kFifo_t;

typedef struct kIPCStruct_t* kLifoHandle_t;
typedef struct kIPCStruct_t* kFifoHandle_t;
typedef struct kIPCStruct_t* kSystemIOHandle_t;

#endif /* KTYPES_H_ */