/*
 * main.c
 *
 * Created: 22.02.2020 21:23:15
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

static uint8_t sdFlags = 1;

void systemd_serviceManager();

void systemd_main()
{
	while(1)
	{
		if (utils_CHECK_BIT(sdFlags, 0)) systemd_serviceManager();
		kernel_yield(CFG_SYSTEMD_YIELD_TICKS);
	}
}

void systemd_pause()
{
	utils_CLEAR_BIT(sdFlags, 0);
}

void systemd_resume()
{
	utils_SET_BIT(sdFlags, 0);
}