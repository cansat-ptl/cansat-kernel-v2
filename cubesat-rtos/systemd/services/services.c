/*
 * servicemgr.c
 *
 * Created: 22.02.2020 19:37:38
 *  Author: Admin
 */

#include <systemd/systemd.h>

static volatile uint8_t sdServiceIndex = 0;
static volatile struct sdServiceStruct_t sdServiceList[CFG_SYSTEMD_MAX_SERVICES];
extern volatile uint8_t sdCallIndex;
extern volatile sdServiceHandle_t sdCallQueue[CFG_SYSTEMD_MAX_SERVICES*2];

kMutex_t sdQueueOpLock;

void systemd_idle();

static inline void systemd_resetServiceByPosition(uint8_t position)
{
	sdServiceList[position].pointer = systemd_idle;
	sdServiceList[position].delay = 0;
	sdServiceList[position].repeatPeriod = 0;
	sdServiceList[position].state = SDSTATE_ACTIVE;
}

sdServiceHandle_t systemd_addService(uint8_t taskType, sdService_t t_ptr, uint16_t t_delay, uint8_t startupState)
{
	struct sdServiceStruct_t dummyService;
	sdServiceHandle_t handle = NULL;
	uint8_t sreg = threads_startAtomicOperation();
	
	if (t_delay == 0) t_delay = 1;

	debug_logMessage(PGM_ON, L_INFO, PSTR("systemd: Registering new service, t_ptr=0x%08X, type=%d, period=%d\r\n"), t_ptr, taskType, t_delay);
	
	dummyService.pointer = t_ptr;
	dummyService.delay = t_delay - 1;
	dummyService.state = startupState;
	if (taskType == SDSERVICE_REPEATED) dummyService.repeatPeriod = t_delay - 1;
	
	for (int i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++) {
		if (sdServiceList[i].pointer == t_ptr || sdServiceList[i].pointer == systemd_idle || sdServiceList[i].pointer == NULL) {
			sdServiceList[i] = dummyService;
			handle = &sdServiceList[i];
			sdServiceIndex++;
			break;
		}
	}
	
	threads_exitCriticalSection(sreg);
	return handle;
}

uint8_t systemd_removeServiceByPosition(uint8_t position)
{
	uint8_t sreg = threads_startAtomicOperation();
	sdServiceIndex--;
	systemd_resetServiceByPosition(position);
	for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
		sdServiceList[j] = sdServiceList[j+1];
	}
	systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);

	threads_endAtomicOperation(sreg);
	return 0;
}

uint8_t systemd_removeService(sdServiceHandle_t handle)
{
	uint8_t exitcode = 1;
	uint8_t sreg = threads_startAtomicOperation();

	uint8_t position = utils_ARRAY_INDEX_FROM_ADDR(sdServiceList, handle, struct sdServiceStruct_t);

	sdServiceIndex--;

	if (position != CFG_SYSTEMD_MAX_SERVICES-1) {
		systemd_resetServiceByPosition(position);
		for (int j = position; j < CFG_SYSTEMD_MAX_SERVICES-1; j++) {
			sdServiceList[j] = sdServiceList[j+1];
		}
		systemd_resetServiceByPosition(CFG_SYSTEMD_MAX_SERVICES-1);
		exitcode = 0;
	}
	
	threads_endAtomicOperation(sreg);
	return exitcode;
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
		if(sdServiceList[i].pointer == systemd_idle || sdServiceList[i].pointer == NULL) continue;
		else {
			if(sdServiceList[i].delay != 0 && sdServiceList[i].state == SDSTATE_ACTIVE)
			sdServiceList[i].delay--;
			else {
				if(sdServiceList[i].state == SDSTATE_ACTIVE){
					systemd_addCall_i((sdServiceHandle_t)&sdServiceList[i]);
					if(sdServiceList[i].repeatPeriod == 0)
						sdServiceList[i].state = SDSTATE_SUSPENDED;
					else
						sdServiceList[i].delay = sdServiceList[i].repeatPeriod;
				}
			}
		}
	}
}