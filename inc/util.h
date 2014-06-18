/*
 * util.h
 *
 *  Created on: 02.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <types.h>

extern void debug_init(void);

extern void debug_show(uint8_t byte);

extern void debug_knightrider();

extern void wait(unsigned int ms);

#endif /* UTIL_H_ */
