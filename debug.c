#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include "debug.h"

#define DBG0	IO004_Handle5
#define DBG1	IO004_Handle6
#define DBG2	IO004_Handle7
#define DBG3	IO004_Handle8
#define DBG4	IO004_Handle9
#define DBG5	IO004_Handle10
#define DBG6	IO004_Handle11
#define DBG7	IO004_Handle12


void debug_show(uint8_t byte)
{
	uint8_t val = byte & 0x01;
	IO004_SetOutputValue(DBG0, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG1, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG2, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG3, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG4, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG5, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG6, val);
	byte >>= 1;

	val = byte & 0x01;
	IO004_SetOutputValue(DBG7, val);
}
