/*
 * memory.h
 *
 * Created: 21.06.2020 21:07:25
 *  Author: Admin
 */


#ifndef MEMORY_H_
#define MEMORY_H_

void memmgr_prepareProtectionRegion(void* pointer, int16_t size);
uint8_t memmgr_checkProtectionRegion(void* pointer, int16_t size);

#endif /* MEMORY_H_ */