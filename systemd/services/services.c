/*
 * servicemgr.c
 *
 * Created: 22.02.2020 19:37:38
 *  Author: Admin
 */ 

#include <systemd/systemd.h>

static volatile uint8_t sdServiceIndex = 0;
static volatile struct sdServiceStruct_t sdServiceQueue[CFG_SYSTEMD_MAX_SERVICES];

static inline void systemd_resetServiceByPosition(uint8_t position)
{
	sdServiceQueue[position].pointer = systemd_idle;
	sdServiceQueue[position].delay = 0;
	sdServiceQueue[position].repeatPeriod = 0;
	sdServiceQueue[position].state = SDSTATE_ACTIVE;
}

uint8_t systemd_addService(uint8_t taskType, sdService_t t_ptr, uint16_t t_delay, uint8_t startupState)
{
	if (t_delay == 0) t_delay = 1;
	
	for (int i = 0; i <= sdServiceIndex; i++) {
		if (sdServiceQueue[i].pointer == t_ptr) {
			sdServiceQueue[i].repeatPeriod = t_delay - 1;
			sdServiceQueue[i].state = startupState;
			if (taskType == SDSERVICE_REPEATED) sdServiceQueue[i].repeatPeriod = t_delay - 1;
			else sdServiceQueue[i].repeatPeriod = 0;
			
			return 0;
		}
	}
	if (sdServiceIndex < CFG_SYSTEMD_MAX_SERVICES) {
		sdServiceQueue[sdServiceIndex].pointer = t_ptr;
		sdServiceQueue[sdServiceIndex].delay = t_delay - 1;
		sdServiceQueue[sdServiceIndex].state = startupState;
		if (taskType == SDSERVICE_REPEATED) sdServiceQueue[sdServiceIndex].repeatPeriod = t_delay - 1;
		else sdServiceQueue[sdServiceIndex].repeatPeriod = 0;
		sdServiceIndex++;
		
		return 0;
	}
	else {
		return 1;
	}
}

uint8_t systemd_removeServiceByPosition(uint8_t position)
{
	sdServiceIndex--;
	systemd_resetServiceByPosition(position);
	for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
		sdServiceQueue[j] = sdServiceQueue[j+1];
	}
	systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);
	
	return 0;
}

uint8_t systemd_removeService(sdService_t t_pointer)
{
	uint8_t position;
		
	sdServiceIndex--;
	for (position = 0; position < CFG_SYSTEMD_MAX_SERVICES-1; position++) {
		if (t_pointer == sdServiceQueue[position].pointer)
			break;
	}
	
	if (position != CFG_SYSTEMD_MAX_SERVICES-1) {
		systemd_resetServiceByPosition(position);
		for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
			sdServiceQueue[j] = sdServiceQueue[j+1];
		}
		systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);
		return 0;
	}
	else {
		return 1;
	}
}

void systemd_clearServiceQueue()
{
	for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++) {
		systemd_resetServiceByPosition(i);
	}
	sdServiceIndex = 0;
}

uint8_t systemd_setServiceState(sdService_t t_pointer, uint8_t state)
{
	for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES-1; i++) {
		if (sdServiceQueue[i].pointer == t_pointer) {
			sdServiceQueue[i].state = state;
			return 0;
		}
	}
	
	return 1;
}