/*
 * calls.c
 *
 * Created: 22.02.2020 20:32:25
 *  Author: Admin
 */

#include <systemd/systemd.h>

volatile uint8_t sdCallIndex = 0u;
volatile sdServiceHandle_t sdCallQueue[CFG_SYSTEMD_MAX_SERVICES*2u];

void systemd_idle()
{
	return;
}

uint8_t systemd_addCall(sdServiceHandle_t handle)
{
	uint8_t exitcode = 1;
	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (sdCallIndex < CFG_SYSTEMD_MAX_SERVICES) {
		sdCallQueue[sdCallIndex] = handle;
		sdCallIndex++;

		exitcode = 0u;
	}

	threads_endAtomicOperation(sreg);
	return exitcode;
}

uint8_t systemd_removeCall()
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	if (sdCallIndex != 0) {
		sdCallIndex--;
		for (kIterator_t i = 0; i < CFG_SYSTEMD_MAX_SERVICES-1; i++) {
			sdCallQueue[i] = sdCallQueue[i+1];
		}
		sdCallQueue[CFG_SYSTEMD_MAX_SERVICES-1] = NULL;
	}
	else {
		sdCallQueue[0] = NULL;
	}

	threads_endAtomicOperation(sreg);
	return 0u;
}

void systemd_clearCallQueue()
{
	kStatusRegister_t sreg = threads_startAtomicOperation();
	for (kIterator_t i = 0; i < CFG_SYSTEMD_MAX_SERVICES; i++) {
		sdCallQueue[i] = NULL;
	}
	sdCallIndex = 0u;
	threads_endAtomicOperation(sreg);
}

void systemd_serviceManager()
{
	while (sdCallQueue[0] != NULL) {
		(sdCallQueue[0] -> pointer)();
		systemd_removeCall();
	}
	systemd_idle();
}