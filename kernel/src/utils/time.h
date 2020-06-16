/*
 * time.h
 *
 * Created: 16.06.2020 16:46:41
 *  Author: Admin
 */ 


#ifndef TIME_H_
#define TIME_H_

struct kSystemTimeStruct_t
{
	uint16_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};

#endif /* TIME_H_ */