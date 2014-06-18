/*
 * ircam.c
 *
 *  Created on: 02.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#include <DAVE3.h>
#include <ircam.h>
#include <i2c.h>
#include <util.h>

void ircam_init(void)
{
	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x30);
	i2c_write(0x01);
	i2c_stop();
	wait(1000);

	/* IR cam sensitivity    |------------------- Block 1 ------------------------|,|- Block 2 -|*/
	/*
	uint8_t wii_lvl_1[11] = { 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x64, 0x00, 0xfe,  0xfd, 0x05 };
	uint8_t wii_lvl_2[11] = { 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x96, 0x00, 0xb4,  0xb3, 0x04 };
	uint8_t wii_lvl_3[11] = { 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xaa, 0x00, 0x64,  0x63, 0x03 };
	uint8_t wii_lvl_4[11] = { 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xc8, 0x00, 0x36,  0x35, 0x03 };
	uint8_t wii_lvl_5[11] = { 0x07, 0x00, 0x00, 0x71, 0x01, 0x00, 0x72, 0x00, 0x20,  0x1f, 0x03 };
	uint8_t high_sens[11] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x41,  0x40, 0x00 };
	uint8_t max_sens[11]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x0c,  0x00, 0x00 };
	*/
	uint8_t high_sens[11] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x41,  0x40, 0x00 };

	uint8_t *ir_settings = high_sens;

	/* START Max sensitivity */
	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x00);
	i2c_write(ir_settings[0]);
	i2c_write(ir_settings[1]);
	i2c_write(ir_settings[2]);
	i2c_write(ir_settings[3]);
	i2c_write(ir_settings[4]);
	i2c_write(ir_settings[5]);
	i2c_write(ir_settings[6]);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x07);
	i2c_write(ir_settings[7]);
	i2c_write(ir_settings[8]);
	i2c_stop();
	wait(1000);

	i2c_start(I2C_ADR, I2C_WRITE);
	i2c_write(0x1A);
	i2c_write(ir_settings[9]);
	i2c_write(ir_settings[10]);
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
	i2c_write(0x36);
	i2c_stop();

	i2c_start(I2C_ADR, I2C_READ);
	for (i = 0; i < 16; i++) {
		data_buf[i] = 0;
	}
	for (i = 0; i < 15; i++) {
		data_buf[i] = i2c_read(I2C_TDF_MRxAck0);
	}
	data_buf[i] = i2c_read(I2C_TDF_MRxAck1);
	i2c_stop();

	p1->x = data_buf[1];
	p1->y = data_buf[2];
	s = data_buf[3];
	p1->x += (s & 0x30) << 4;
	p1->y += (s & 0xC0) << 2;
	p1->size = (s & 0x0F);

	p2->x = data_buf[4];
	p2->y = data_buf[5];
	s = data_buf[6];
	p2->x += (s & 0x30) << 4;
	p2->y += (s & 0xC0) << 2;
	p2->size = (s & 0x0F);

	p3->x = data_buf[7];
	p3->y = data_buf[8];
	s = data_buf[9];
	p3->x += (s & 0x30) << 4;
	p3->y += (s & 0xC0) << 2;
	p3->size = (s & 0x0F);

	p4->x = data_buf[10];
	p4->y = data_buf[11];
	s = data_buf[12];
	p4->x += (s & 0x30) << 4;
	p4->y += (s & 0xC0) << 2;
	p4->size = (s & 0x0F);
}
