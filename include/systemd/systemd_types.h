/*
 * systemd_types.h
 *
 * Created: 22.02.2020 19:47:11
 *  Author: Admin
 */


#ifndef SYSTEMD_TYPES_H_
#define SYSTEMD_TYPES_H_

typedef void (*sdService_t)(void);
typedef struct sdServiceStruct_t * sdServiceHandle_t;

struct sdServiceStruct_t
{
	sdService_t pointer;
	uint16_t delay;
	uint16_t repeatPeriod;
	uint8_t state;
};

#endif /* SYSTEMD-TYPES_H_ */