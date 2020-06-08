/*
 * timers.c
 *
 * Created: 22.02.2020 20:51:53
 *  Author: Admin
 */ 
#include <kernel/kernel.h>

static volatile struct kTimerStruct_t kTimerList[CFG_MAX_TIMER_COUNT];
static volatile uint8_t kTimerIndex = 0;

kTimerHandle_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_period)
{
	kTimerHandle_t dummyHandle = NULL;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	for (kIterator_t i = 0; i < CFG_MAX_TIMER_COUNT; i++) {
		if (kTimerList[i].tsrPointer == t_pointer || kTimerList[i].tsrPointer == NULL) {
			kTimerList[i].tsrPointer = t_pointer;
			kTimerList[i].period = t_period;
			kTimerList[i].repeatPeriod = t_period;
			dummyHandle = &kTimerList[i];
			kTimerIndex++;
			break;
		}
	}

	threads_endAtomicOperation(sreg);
	return dummyHandle;
}

void kernel_removeTimer(kTimerHandle_t handle)
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	
	if (handle != NULL) handle -> tsrPointer = NULL;
	
	threads_endAtomicOperation(sreg);
}

inline void kernel_timerService()
{
	for(int i = 0; i < CFG_MAX_TIMER_COUNT; i++){
		if(kTimerList[i].tsrPointer == NULL) continue;
		else {
			if(kTimerList[i].period != 0)
			kTimerList[i].period--;
			else {
				(kTimerList[i].tsrPointer)();
				kTimerList[i].period = kTimerList[i].repeatPeriod;
			}
		}
	}	
}