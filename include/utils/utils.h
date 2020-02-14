/*
 * utils.h
 *
 * Created: 14.02.2020 20:04:46
 *  Author: Admin
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#define utils_SET_BIT(x,y) x |= (1 << y)
#define utils_CLEAR_BIT(x,y) x &= ~(1 << y)
#define utils_CHECK_BIT(x,y) ((0u == (x & (1 << y))) ? 0u : 1u)
#define utils_SWITCH_BIT(x,y) (x ^= (1 << y))
#define utils_WRITE_BIT(x,y,z) x ^= (-1 * z ^ x) & (1 << y);

#endif /* UTILS_H_ */