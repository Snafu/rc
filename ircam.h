/*
 * ircam.h
 *
 *  Created on: 02.04.2014
 *      Author: Snafu
 */

#ifndef IRCAM_H_
#define IRCAM_H_

typedef struct {
	uint16_t x;
	uint16_t y;
	uint8_t size;
} ir_point_t;

extern void ircam_init(void);

extern void ircam_read(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4);

#endif /* IRCAM_H_ */
