#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <util.h>

#define DBG0	IO004_Handle5
#define DBG1	IO004_Handle6
#define DBG2	IO004_Handle7
#define DBG3	IO004_Handle8
#define DBG4	IO004_Handle9
#define DBG5	IO004_Handle10
#define DBG6	IO004_Handle11
#define DBG7	IO004_Handle12


static void DelayHandler(void* t);

static volatile bool delayExpired = FALSE;

void debug_init(void)
{
	for(int i = 0; i < 8; i++) {
		debug_show(1<<i);
		wait(500);
	}
	debug_show(0xFF);
	wait(500);
	debug_show(0x55);
	wait(500);
	debug_show(0xAA);
	wait(500);
	debug_show(0x00);
	wait(500);
}

void debug_knightrider()
{
	static uint8_t dir = 0;
	static uint16_t state = 1;
	debug_show(state);
	if(dir == 0)
		state <<= 1;
	else
		state >>= 1;
	if(state == 0x0100) {
		state = 0x40;
		dir = 1;
	} else if(state == 0x00) {
		state = 0x02;
		dir = 0;
	}
}

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

void wait(unsigned int ms)
{
	handle_t handle;
	handle = SYSTM001_CreateTimer(ms, SYSTM001_ONE_SHOT, DelayHandler, NULL);
	delayExpired = FALSE;
	if(handle)
		(void) SYSTM001_StartTimer(handle);
	while(!delayExpired);
	(void) SYSTM001_DeleteTimer(handle);
}

void DelayHandler(void* t)
{
	delayExpired = TRUE;
}
