/*
 * Main.c
 *
 *  Created on: 22.03.2014
 *      Author: Snafu
 */


#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <string.h>
#include <stdio.h>
#include <ircam.h>
#include <car.h>
#include <util.h>
#include <bluetooth.h>

#define CAR_INIT_ERROR	0x8f

char debug[100];

void car_test(void);

int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps
	debug_init();

	if(car_init()) {
		debug_show(CAR_INIT_ERROR);
		while(1);
	}

	car_test();

	strcpy(debug, "Starting IR init\r\n");
	bt_puts(debug);
	ircam_init();

	ir_point_t p1, p2, p3, p4;
	strcpy(debug, "Polling data\r\n");
	bt_puts(debug);
	while(1)
	{
		ircam_read(&p1, &p2, &p3, &p4);
		sprintf(debug, "%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d\r\n",
				p1.x, p1.y, p1.size,
				p2.x, p2.y, p2.size,
				p3.x, p3.y, p3.size,
				p4.x, p4.y, p4.size);
		bt_puts(debug);
		debug_knightrider();
		wait(100);
	}
	return 0;
}

void car_test(void)
{
	car_steer(-50);
	wait(500);
	car_steer(-100);
	wait(500);
	car_steer(50);
	wait(500);
	car_steer(100);
	wait(500);
	car_steer(0);

	wait(2000);
	car_throttle(-30);
	wait(500);
	car_throttle(30);
	wait(500);
	car_throttle(0);
}
