/*
 * kernel.h
 *
 * Created: 16.06.2020 16:16:23
 *  Author: Admin
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

#define ERR_GENERIC -1
#define ERR_NULLPTR -2
#define ERR_MEMORY_VIOLATION -3

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

#define KPRIO_IDLE 0
#define KPRIO_REALTIME 255

typedef void kTask;
typedef void (*kTask_t)(void*);

struct kTaskStruct;
typedef volatile struct kTaskStruct_t* kTaskHandle_t;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_SLEEPING, KSTATE_BLOCKED, KSTATE_READY, KSTATE_RUNNING} kTaskState_t;
typedef enum {KEVENT_NONE, KEVENT_FIRED} kEventState_t;
typedef enum {KTASK_USER, KTASK_SYSTEM} kTaskType_t;
typedef enum {KTIMER_SINGLERUN, KTASK_REPEATED} kTimerType_t;
typedef enum {KLOCK_SEMAPHORE, KLOCK_MUTEX, KLOCK_SEMAPHORE_RECURSIVE} kLockType_t;

#endif /* KERNEL_H_ */