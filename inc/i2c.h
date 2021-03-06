/*
 * i2c.h
 *
 *  Created on: 02.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#ifndef I2C_H_
#define I2C_H_

#include <types.h>

#define I2C_MST	(&I2C001_Handle0)
#define	I2C_ADR	0xB0		// ( 0x58 << 1 )

extern bool i2c_start(uint8_t addr, uint8_t mode);

extern void i2c_write(uint8_t data);

extern uint8_t i2c_read(uint8_t mode);

extern void i2c_stop(void);

extern void i2c_rx_handler(void);

#endif /* I2C_H_ */
