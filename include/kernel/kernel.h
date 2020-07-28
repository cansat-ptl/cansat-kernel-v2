/*
 * kernel.h
 *
 * Created: 21.06.2020 20:22:55
 *  Author: Admin
 */


#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>
//TODO: full include

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);

uint64_t kernel_getUptime();
uint8_t kernel_getSystemStatus();

#endif /* KERNEL_H_ */