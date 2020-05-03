/*
 * time.c
 *
 * Created: 27.01.2020 20:22:53
 *  Author: Admin
 */ 

#include <kernel/kernel.h>

static struct kSystemTime_t kSystemTime;
/*
struct kSystemTime_t {
	uint16_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};
*/


extern volatile uint64_t __e_time;

uint64_t kernel_getUptime()
{
	return __e_time;
}

void time_updateSystemTime()
{
	kSystemTime.milliseconds = (uint16_t)(__e_time % 1000);
	kSystemTime.seconds = (uint8_t)((__e_time / 1000) % 60);
	kSystemTime.minutes = (uint8_t)((__e_time / 1000 / 60) % 60);
	kSystemTime.hours = (uint8_t)((__e_time / 1000 / 60 / 60) % 24);
	kSystemTime.days = (uint16_t)(__e_time / 1000 / 60 / 60 / 24);
}

uint16_t time_getMilliseconds()
{
	return kSystemTime.milliseconds;
}

uint8_t time_getSeconds()
{
	return kSystemTime.seconds;
}

uint8_t time_getMinutes()
{
	return kSystemTime.minutes;
}

uint8_t time_getHours()
{
	return kSystemTime.hours;
}

uint16_t time_getDays()
{
	return kSystemTime.days;
}




