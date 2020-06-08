/*
 * porttypes.h
 *
 * Created: 08.06.2020 20:30:54
 *  Author: Admin
 */ 


#ifndef PORTTYPES_H_
#define PORTTYPES_H_

#include <stdint.h>
#include <stddef.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef uint32_t size_t;
#endif

typedef uint32_t kPointerValue_t;
typedef volatile uint8_t *kStackPtr_t;

typedef int16_t kIterator_t;
typedef int16_t kReturnValue_t;
typedef int16_t kError_t;

typedef uint8_t byte;
typedef uint8_t kRegister_t;
typedef uint8_t kStatusRegister_t;

typedef uint16_t kStackSize_t;
typedef uint16_t kTaskTicks_t;
typedef uint16_t kPid_t;

#endif /* PORTTYPES_H_ */