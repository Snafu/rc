/*
 * i2c.c
 *
 *  Created on: 02.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#include <DAVE3.h>
#include <i2c.h>

bool i2c_start(uint8_t addr, uint8_t mode)
{
	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MStart;
	data1.Data1.Data = (addr | mode);
	while(!I2C001_WriteData(I2C_MST,&data1));

	return TRUE;
}

void i2c_write(uint8_t data)
{
	//while(I2C001_GetFlagStatus(I2C_MST,I2C001_FLAG_TBIF) != I2C001_SET);
	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MTxData;
	data1.Data1.Data = data;
	while(!I2C001_WriteData(I2C_MST,&data1));
}

uint8_t i2c_read(uint8_t mode)
{
	// Read one data from slave
	I2C001_DataType data1;
	data1.Data1.TDF_Type = mode;
	data1.Data1.Data = ubyteFF;
	while(!I2C001_WriteData(&I2C001_Handle0,&data1));

	uint16_t DataReceive1 = 0x0000;
	// Read receive buffer, put the data in DataReceive1
	while(!I2C001_ReadData(I2C_MST,&DataReceive1));

	return (uint8_t) DataReceive1;
}

void i2c_stop(void)
{
	// Stop condition by the master
	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MStop;
	data1.Data1.Data = ubyteFF;
	while(!I2C001_WriteData(I2C_MST,&data1));
}
