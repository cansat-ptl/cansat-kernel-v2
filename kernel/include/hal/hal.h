/*
 * hal.h
 *
 * Created: 25.05.2019 20:44:34
 *  Author: ThePetrovich
 */

#ifndef HAL_H_
#define HAL_H_

#define HAL_MOD_VER "0.2.2-staging"
#define HAL_MOD_TIMESTAMP __TIMESTAMP__

#include <avr/pgmspace.h>
#include <kerneldefs.h>
#include "../../hal_pins.h"
#include "../../kernel_config.h"

#ifndef hal_REBOOT
	#define hal_REBOOT() while (1){;}
#endif

#endif /* HAL_H_ */