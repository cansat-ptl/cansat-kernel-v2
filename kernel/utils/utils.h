/*
 * utils.h
 *
 * Created: 26.06.2020 18:01:15
 *  Author: Admin
 */


#ifndef UTILS_INTERNAL_H_
#define UTILS_INTERNAL_H_

#define UTIL_VER "0.0.1-bleeding"
#define UTIL_TIMESTAMP __TIMESTAMP__

#define utils_SET_BIT(x,y) ((x) |= (1u << (y)))
#define utils_CLEAR_BIT(x,y) ((x) &= ~(1u << (y)))
#define utils_CHECK_BIT(x,y) (((0u == ((x) & (1u << (y)))) ? 0u : 1u))
#define utils_SWITCH_BIT(x,y) (((x) ^= (1u << (y))))
#define utils_WRITE_BIT(x,y,z) ((x) = ((x) & ~(1u << (y))) | ((z) << (y)));

#define utils_ARRAY_INDEX_FROM_ADDR(base, addr, type) (((uint16_t)(addr)-(uint16_t)(base))/sizeof(type))

#endif /* UTILS_H_ */