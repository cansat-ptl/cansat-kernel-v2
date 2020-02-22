/*
 * timers.c
 *
 * Created: 22.02.2020 20:51:53
 *  Author: Admin
 */ 
#include <kernel.h>

static volatile struct kTimerStruct_t kTimerQueue[CFG_MAX_TIMER_COUNT];
static volatile uint8_t kTimerIndex = 0;

uint8_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_delay)
{
	for(int i = 0; i <= kTimerIndex; i++){
		if(kTimerQueue[i].tsrPointer == t_pointer){
			kTimerQueue[i].period = t_delay;
			kTimerQueue[i].repeatPeriod = t_delay;
			
			return 0;
		}
	}
	if(kTimerIndex < CFG_MAX_TIMER_COUNT){
		kTimerIndex++;
		kTimerQueue[kTimerIndex].tsrPointer = t_pointer;
		kTimerQueue[kTimerIndex].period = t_delay;
		kTimerQueue[kTimerIndex].repeatPeriod = t_delay;
		
		return 0;
	}
	else {
		return CFG_MAX_TIMER_COUNT;
	}
}

uint8_t kernel_removeTimer(kTimerISR_t t_pointer)
{
	uint8_t position;
	
	kTimerIndex--;
	for(position = 0; position < CFG_MAX_TIMER_COUNT-1; position++){
		if(t_pointer == kTimerQueue[position].tsrPointer)
		break;
	}
	
	if(position != CFG_MAX_TIMER_COUNT-1){
		kTimerQueue[position].tsrPointer = NULL;
		kTimerQueue[position].period = 0;
		for(int j = position; j < CFG_MAX_TIMER_COUNT-1; j++){
			kTimerQueue[j] = kTimerQueue[j+1];
		}
		kTimerQueue[CFG_MAX_TIMER_COUNT-1].tsrPointer = NULL;
		kTimerQueue[CFG_MAX_TIMER_COUNT-1].period = 0;
		
		return 0;
	}
	
	return 0;
}

inline void kernel_timerService()
{
	for(int i = 0; i < CFG_MAX_TIMER_COUNT; i++){
		if(kTimerQueue[i].tsrPointer == NULL) continue;
		else {
			if(kTimerQueue[i].period != 0)
			kTimerQueue[i].period--;
			else {
				(kTimerQueue[i].tsrPointer)();
				kTimerQueue[i].period = kTimerQueue[i].repeatPeriod;
			}
		}
	}
}