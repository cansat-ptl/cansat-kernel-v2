/*
 * lowlevel.h
 *
 * Created: 18.06.2020 0:28:31
 *  Author: Admin
 */ 


#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_

typedef volatile uint8_t kSpinlock_t;
typedef volatile kSpinlock_t* kSpinlockHandle_t;

uint16_t threads_startCriticalSection();
void threads_endCriticalSection(uint16_t kflags);

kStatusRegister_t threads_startAtomicOperation();
void threads_endAtomicOperation(kStatusRegister_t sreg);

void threads_spinlockAcquire(kSpinlock_t* spinlock);
void threads_spinlockRelease(kSpinlock_t* spinlock);

#endif /* LOWLEVEL_H_ */