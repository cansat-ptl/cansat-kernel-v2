/*
 * timers.h
 *
 * Created: 13.02.2020 19:41:47
 *  Author: Admin
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

void hal_stopTimer1A();
void hal_startTimer1A();
void hal_setupTimer1A(uint8_t prescaler);
void hal_stopTimer0();
void hal_startTimer0();
void hal_setupTimer0(uint8_t prescaler);

#endif /* TIMERS_H_ */