/*
 * timers.h
 *
 * Created: 21.06.2020 20:18:15
 *  Author: Admin
 */


#ifndef TIMERS_H_
#define TIMERS_H_

kTimerHandle_t kernel_setTimer(kTimerISR_t t_pointer, uint32_t t_period);
void kernel_removeTimer(kTimerHandle_t handle);
void kernel_timerService();

#endif /* TIMERS_H_ */