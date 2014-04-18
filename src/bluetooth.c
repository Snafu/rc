/*
 * bluetooth.c
 *
 *  Created on: 07.04.2014
 *      Author: Matthias
 */

#include <DAVE3.h>
#include <bluetooth.h>
#include <string.h>

#define BT_HANDLE	UART001_Handle0

void bt_init(void)
{
	bt_puts("\r\n");
}

void bt_puts(const char *s)
{
	bt_write((const uint8_t*) s, strlen(s));
}

void bt_write(const uint8_t *data, int len)
{
	uint8_t *d = (uint8_t*) data;
	for(; len > 0; --len) {
		while(!UART001_WriteDataBytes(&BT_HANDLE, d, 1));
		d++;
	}
}
