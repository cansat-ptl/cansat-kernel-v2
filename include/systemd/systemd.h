/*
 * systemd.h
 *
 * Created: 22.02.2020 19:47:26
 *  Author: Admin
 */ 


#ifndef SYSTEMD_H_
#define SYSTEMD_H_

#include <kernel.h>
#include <systemd/systemd_types.h>

#include "systemd_config.h"

#define SDSERVICE_SINGLERUN 0
#define SDSERVICE_REPEATED 1

#define SDSTATE_ACTIVE 1
#define SDSTATE_SUSPENDED 0

void systemd_idle();

uint8_t systemd_addService(uint8_t taskType, sdService_t t_ptr, uint16_t t_delay, uint8_t startupState);
uint8_t systemd_removeService(sdService_t t_pointer);
uint8_t systemd_setServiceState(sdService_t t_pointer, uint8_t state);
uint8_t systemd_addCall(sdService_t t_ptr);
uint8_t systemd_removeCall();

#endif /* SYSTEMD_H_ */