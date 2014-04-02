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

#define CAR_INIT_ERROR	0x8f

char debug[80];

void car_test(void);

int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps
	debug_init();

	if(car_init()) {
		debug_show(CAR_INIT_ERROR);
		while(1);
	}

	//car_test();

	strcpy(debug, "Starting IR init");
	ircam_init();

	ir_point_t p1, p2, p3, p4;
	strcpy(debug, "Polling data");
	while(1)
	{
		ircam_read(&p1, &p2, &p3, &p4);
		sprintf(debug, "%d,%d (%d)  -  %d,%d (%d)\n",
				p1.x, p1.y, p1.size,
				p2.x, p2.y, p2.size);
		wait(2000);
	}
	return 0;
}

void car_test(void)
{
	car_steer(-50);
	wait(250);
	car_steer(-100);
	wait(250);
	car_steer(50);
	wait(250);
	car_steer(100);
	wait(250);
	car_steer(0);

	wait(1000);
	car_throttle(-30);
	wait(250);
	car_throttle(30);
	wait(250);
	car_throttle(0);
}
