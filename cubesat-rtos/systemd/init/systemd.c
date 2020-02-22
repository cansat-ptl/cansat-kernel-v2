/*
 * main.c
 *
 * Created: 22.02.2020 21:23:15
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

void systemd_main()
{
	while(1)
	{
		systemd_serviceManager();
		kernel_yield(CFG_SYSTEMD_YIELD_TICKS);
	}
}