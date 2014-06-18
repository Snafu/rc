/*
 * BLUETOOTH.h
 *
 *  Created on: 07.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

extern void bt_init(void);

extern void bt_puts(const char *s);

extern void bt_write(const uint8_t *data, int len);

#endif /* BLUETOOTH_H_ */
