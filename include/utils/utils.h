/*
 * utils.h
 *
 * Created: 14.02.2020 20:04:46
 *  Author: Admin
 */


#ifndef UTILS_H_
#define UTILS_H_

#define UTIL_VER "0.0.1-bleeding"
#define UTIL_TIMESTAMP __TIMESTAMP__

#define utils_SET_BIT(x,y) x |= (1 << y)
#define utils_CLEAR_BIT(x,y) x &= ~(1 << y)
#define utils_CHECK_BIT(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define utils_SWITCH_BIT(x,y) (x ^= (1 << y))
#define utils_WRITE_BIT(x,y,z) x ^= (-1 * z ^ x) & (1 << y);

#define utils_ARRAY_INDEX_FROM_ADDR(base, addr, type) (((uint16_t)(addr)-(uint16_t)(base))/sizeof(type))

#endif /* UTILS_H_ */