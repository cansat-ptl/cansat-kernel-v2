/*
 * servicemgr.c
 *
 * Created: 22.02.2020 19:37:38
 *  Author: Admin
 */

#include <systemd/systemd.h>

static volatile uint8_t sdServiceIndex = 0;
static volatile struct sdServiceStruct_t sdServiceQueue[CFG_SYSTEMD_MAX_SERVICES];
extern volatile uint8_t sdCallIndex;
extern volatile sdServiceHandle_t sdCallQueue[CFG_SYSTEMD_MAX_SERVICES*2];

kMutex_t sdQueueOpLock;

void systemd_idle();

static inline void systemd_resetServiceByPosition(uint8_t position)
{
	sdServiceQueue[position].pointer = systemd_idle;
	sdServiceQueue[position].delay = 0;
	sdServiceQueue[position].repeatPeriod = 0;
	sdServiceQueue[position].state = SDSTATE_ACTIVE;
}

uint8_t systemd_addService(uint8_t taskType, sdService_t t_ptr, uint16_t t_delay, uint8_t startupState)
{
	uint8_t sreg = threads_startAtomicOperation();
	if (t_delay == 0) t_delay = 1;

	debug_logMessage(PGM_ON, L_INFO, PSTR("systemd: Registering new service, t_ptr=0x%08X, type=%d, period=%d\r\n"), t_ptr, taskType, t_delay);
	
	for (int i = 0; i <= sdServiceIndex; i++) {
		if (sdServiceQueue[i].pointer == t_ptr) {
			sdServiceQueue[i].repeatPeriod = t_delay - 1;
			sdServiceQueue[i].state = startupState;
			if (taskType == SDSERVICE_REPEATED) sdServiceQueue[i].repeatPeriod = t_delay - 1;
			else sdServiceQueue[i].repeatPeriod = 0;

			threads_endAtomicOperation(sreg);
			debug_logMessage(PGM_ON, L_INFO, PSTR("systemd: Found existing service, successfully reset\r\n"));
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

		threads_endAtomicOperation(sreg);
		debug_logMessage(PGM_ON, L_INFO, PSTR("systemd: Successfully registered new service, idx=%d\r\n"), sdServiceIndex);
		return 0;
	}
	else {
		threads_endAtomicOperation(sreg);
		debug_puts(L_ERROR, PSTR("systemd: Failed to register service\r\n"));
		return 1;
	}
}

uint8_t systemd_removeServiceByPosition(uint8_t position)
{
	uint8_t sreg = threads_startAtomicOperation();
	sdServiceIndex--;
	systemd_resetServiceByPosition(position);
	for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
		sdServiceQueue[j] = sdServiceQueue[j+1];
	}
	systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);

	threads_endAtomicOperation(sreg);
	return 0;
}

uint8_t systemd_removeService(sdServiceHandle_t handle)
{
	uint8_t sreg = threads_startAtomicOperation();

	uint8_t position = utils_ARRAY_INDEX_FROM_ADDR(sdServiceQueue, handle, struct sdServiceStruct_t);

	sdServiceIndex--;

	if (position != CFG_SYSTEMD_MAX_SERVICES-1) {
		systemd_resetServiceByPosition(position);
		for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
			sdServiceQueue[j] = sdServiceQueue[j+1];
		}
		systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);

		threads_endAtomicOperation(sreg);
		return 0;
	}
	else {
		threads_endAtomicOperation(sreg);
		return 1;
	}
}

void systemd_clearServiceQueue()
{
	uint8_t sreg = threads_startAtomicOperation();
	for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++) {
		systemd_resetServiceByPosition(i);
	}
	sdServiceIndex = 0;
	threads_endAtomicOperation(sreg);
}

void systemd_setServiceState(sdServiceHandle_t handle, uint8_t state)
{
	uint8_t sreg = threads_startAtomicOperation();

	if (handle != NULL) handle -> state = state;
	
	threads_endAtomicOperation(sreg);
}

static inline void systemd_addCall_i(sdServiceHandle_t handle)
{
	if (sdCallIndex < CFG_SYSTEMD_MAX_SERVICES) {
		sdCallQueue[sdCallIndex] = handle;
		sdCallIndex++;
	}
}

void systemd_tick()
{
	for(int i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++){
		if(sdServiceQueue[i].pointer == systemd_idle || sdServiceQueue[i].pointer == NULL) continue;
		else {
			if(sdServiceQueue[i].delay != 0 && sdServiceQueue[i].state == SDSTATE_ACTIVE)
			sdServiceQueue[i].delay--;
			else {
				if(sdServiceQueue[i].state == SDSTATE_ACTIVE){
					systemd_addCall_i((sdServiceHandle_t)&sdServiceQueue[i]);
					if(sdServiceQueue[i].repeatPeriod == 0)
						sdServiceQueue[i].state = SDSTATE_SUSPENDED;
					else
						sdServiceQueue[i].delay = sdServiceQueue[i].repeatPeriod;
				}
			}
		}
	}
}