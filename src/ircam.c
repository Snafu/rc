/*
 * ircam.c
 *
 *  Created on: 02.04.2014
 *      Author: Snafu
 */
#include <DAVE3.h>
#include <ircam.h>
#include <i2c.h>
#include <util.h>


void ircam_init(void)
{
	uint8_t p0,p1,p2,p3;


	// IR sensor initialize
	/*
	 * From PVision.cpp
		Write_2bytes(0x30,0x01); delay(10);
		Write_2bytes(0x30,0x08); delay(10);
		Write_2bytes(0x06,0x90); delay(10);
		Write_2bytes(0x08,0xC0); delay(10);
		Write_2bytes(0x1A,0x40); delay(10);
		Write_2bytes(0x33,0x33); delay(10);
		delay(100);
	 */

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x30);
	i2c_write(0x01);
	i2c_stop();
	wait(1000);

	//p0=0x72; p1=0x20; p2=0x1F; p3=0x03; // sensitivity
	//p0=0xC8; p1=0x36; p2=0x35; p3=0x03;
	  p0=0xAA; p1=0x64; p2=0x63; p3=0x03; // vorher
	//p0=0x96; p1=0xB4; p2=0xB3; p3=0x04;
	//p0=0x96; p1=0xFE; p2=0xFE; p3=0x05;
	/*

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x00);
	i2c_write(0x02);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x71);
	i2c_write(0x01);
	i2c_write(0x00);
	i2c_write(p0);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x07);
	i2c_write(0x00);
	i2c_write(p1);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x1A);
	i2c_write(p2);
	i2c_write(p3);
	i2c_stop();
	wait(1000);
	*/

	/* START Max sensitivity */
	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_write(0xff);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x07);
	i2c_write(0x00);
	i2c_write(0x0c);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x1A);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_stop();
	wait(1000);

	/* END Max sensitivity */

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x33);
	i2c_write(0x03);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x30);
	i2c_write(0x08);
	i2c_stop();
	wait(1000);
}

void ircam_read(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4)
{
	int i;
	uint8_t s;
	uint8_t data_buf[16];


	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write( 0x36 );
	i2c_stop();

	i2c_start(I2C_ADR, I2C_READ);
	for (i=0;i<16;i++) { data_buf[i]=0; }
	for (i=0;i<15;i++) {
		data_buf[i] = i2c_read(I2C_TDF_MRxAck0);
	}
	data_buf[i] = i2c_read(I2C_TDF_MRxAck1);
	i2c_stop();


	p1->x = data_buf[1];
	p1->y = data_buf[2];
	s = data_buf[3];
	p1->x += (s & 0x30) <<4;
	p1->y += (s & 0xC0) <<2;
	p1->size = (s & 0x0F);

	p2->x = data_buf[4];
	p2->y = data_buf[5];
	s = data_buf[6];
	p2->x += (s & 0x30) <<4;
	p2->y += (s & 0xC0) <<2;
	p2->size = (s & 0x0F);

	p3->x = data_buf[7];
	p3->y = data_buf[8];
	s = data_buf[9];
	p3->x += (s & 0x30) <<4;
	p3->y += (s & 0xC0) <<2;
	p3->size = (s & 0x0F);

	p4->x = data_buf[10];
	p4->y = data_buf[11];
	s = data_buf[12];
	p4->x += (s & 0x30) <<4;
	p4->y += (s & 0xC0) <<2;
	p4->size = (s & 0x0F);
}
