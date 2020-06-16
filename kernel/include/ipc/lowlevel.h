/*
 * lowlevel.h
 *
 * Created: 16.06.2020 16:31:41
 *  Author: Admin
 */ 


#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_

#include <platform/ptypes.h>

typedef volatile uint8_t kSpinlock_t;
typedef volatile kSpinlock_t* kSpinlockHandle_t;

uint16_t threads_startCriticalSection();
void threads_endCriticalSection(uint16_t kflags);

kStatusRegister_t threads_startAtomicOperation();
void threads_endAtomicOperation(kStatusRegister_t sreg);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* LOWLEVEL_H_ */