/*
 * calls.c
 *
 * Created: 22.02.2020 20:32:25
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

static volatile uint8_t sdCallIndex = 0;
static volatile sdService_t sdCallQueue[CFG_SYSTEMD_MAX_SERVICES];

void systemd_idle()
{
	return;
}

static inline void systemd_addCall_i(sdService_t t_ptr)
{
	if (sdCallIndex < CFG_SYSTEMD_MAX_SERVICES) {
		sdCallQueue[sdCallIndex] = t_ptr;
		sdCallIndex++;
	}
}

uint8_t systemd_addCall(sdService_t t_ptr)
{
	if (sdCallIndex < CFG_SYSTEMD_MAX_SERVICES) {
		sdCallQueue[sdCallIndex] = t_ptr;
		sdCallIndex++;
		
		return 0;
	}
	else {
		return 1;
	}
}

uint8_t systemd_removeCall()
{
	if (sdCallIndex != 0) {
		sdCallIndex--;
		for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES-1; i++) {
			sdCallQueue[i] = sdCallQueue[i+1];
		}
		sdCallQueue[CFG_SYSTEMD_MAX_SERVICES-1] = systemd_idle;
	}
	else {
		sdCallQueue[0] = systemd_idle;
	}
	
	return 0;
}

void systemd_clearCallQueue()
{
	for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++) {
		sdCallQueue[i] = systemd_idle;
	}
	sdCallIndex = 0;
}

void systemd_serviceManager()
{
	if(sdCallQueue[0] != systemd_idle){
		(sdCallQueue[0])();
		systemd_removeCall();
	}
	systemd_idle();
}