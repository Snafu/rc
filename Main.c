/*
 * Main.c
 *
 *  Created on: 22.03.2014
 *      Author: Snafu
 */


#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <string.h>
#include <stdio.h>
#include "i2c.h"

char debug[80];
static volatile bool delayExpired = FALSE;

void ir_init(void);
void ir_read(void);
void wait(unsigned int ms);
void DelayHandler(void* t);


int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps


	strcpy(debug, "Starting IR init");
	ir_init();

	strcpy(debug, "Polling data");
	while(1)
	{
		ir_read();
		wait(2000);
	}
	return 0;
}

void ir_init(void)
{
		uint8_t p0,p1,p2,p3;


	    // IR sensor initialize

		//p0=0x72; p1=0x20; p2=0x1F; p3=0x03; // sensitivity
		//p0=0xC8; p1=0x36; p2=0x35; p3=0x03;
		  p0=0xAA; p1=0x64; p2=0x63; p3=0x03;
		//p0=0x96; p1=0xB4; p2=0xB3; p3=0x04;
		//p0=0x96; p1=0xFE; p2=0xFE; p3=0x05;

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x30 ); i2c_write( 0x01 );
		i2c_stop();
		wait(1000);

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x00 );
		i2c_write( 0x02 ); i2c_write( 0x00 ); i2c_write( 0x00 ); i2c_write( 0x71 );
		i2c_write( 0x01 ); i2c_write( 0x00 ); i2c_write( p0 );
		i2c_stop();
		wait(1000);

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x07 ); i2c_write( 0x00 ); i2c_write( p1 );
		i2c_stop();
		wait(1000);

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x1A ); i2c_write( p2 ); i2c_write( p3 );
		i2c_stop();
		wait(1000);

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x33 ); i2c_write( 0x03 );
		i2c_stop();
		wait(1000);

		i2c_start(I2C_ADR,I2C_WRITE);
		i2c_write( 0x30 ); i2c_write( 0x08 );
		i2c_stop();
		wait(1000);

}

void ir_read(void)
{

	int s;
	uint8_t data_buf[16];
	int Ix1,Iy1,Ix2,Iy2;
	int Ix3,Iy3,Ix4,Iy4;
	int i;


	i2c_start(I2C_ADR,I2C_WRITE);
	i2c_write( 0x36 );
	i2c_stop();

	i2c_start(I2C_ADR,I2C_READ);
	for (i=0;i<16;i++) { data_buf[i]=0; }
	for (i=0;i<16;i++) {
		if (i!=15) {
			data_buf[i] = i2c_read(I2C_TDF_MRxAck0);
		} else {
			data_buf[i] = i2c_read(I2C_TDF_MRxAck1);
		}
	}
	i2c_stop();


	Ix1 = data_buf[1]; Iy1 = data_buf[2]; s = data_buf[3];
	Ix1 += (s & 0x30) <<4; Iy1 += (s & 0xC0) <<2;
	Ix2 = data_buf[4]; Iy2 = data_buf[5]; s = data_buf[6];
	Ix2 += (s & 0x30) <<4; Iy2 += (s & 0xC0) <<2;
	Ix3 = data_buf[7]; Iy3 = data_buf[8]; s = data_buf[9];
	Ix3 += (s & 0x30) <<4; Iy3 += (s & 0xC0) <<2;
	Ix4 = data_buf[10]; Iy4 = data_buf[11]; s = data_buf[12];
	Ix4 += (s & 0x30) <<4; Iy4 += (s & 0xC0) <<2;

	sprintf(debug, "%d,%d  -  %d,%d\n", Ix1, Iy1, Ix2, Iy2);

}

void wait(unsigned int ms)
{
	handle_t handle;
	handle = SYSTM001_CreateTimer(ms,SYSTM001_ONE_SHOT,DelayHandler,NULL);
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
