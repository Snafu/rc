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

#define IR_DIFF			50	// maximum ir_point_t x diff for matching set
#define IR_MAX_SIZE		3	// maximum ir_point_t size
#define IR_LED_DIST		80	// distance between LEDs in mm
#define IR_LED_POS_MIN	521	// ir_point_t y diff @ 200 mm
#define IR_LED_POS_SET	301 // ir_point_t y diff @ 300 mm
#define IR_LED_POS_MAX	153	// ir_point_t y diff @ 500 mm

char debug[100];

int s(int diff);
void car_test(void);
void car_control(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4);

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
	int counter = 0;
	while(1)
	{
		ircam_read(&p1, &p2, &p3, &p4);
		car_control(&p1, &p2, &p3, &p4);

		sprintf(debug, "#%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d\r\n",
				p1.x, p1.y, p1.size,
				p2.x, p2.y, p2.size,
				p3.x, p3.y, p3.size,
				p4.x, p4.y, p4.size);
		bt_puts(debug);

		if(counter++ == 4) {
			debug_knightrider();
			counter = 0;
		}
		wait(10);
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

int s(int diff)
{
	return 100000/diff;
}

void car_control(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4)
{
	ir_point_t *p[4] = {p1, p2, p3, p4};
	ir_point_t *top = NULL;
	ir_point_t *bottom = NULL;

	for(int i = 0; i < 4; i++) {
		if(bottom != NULL) {
			break;
		}
		if(p[i]->size < IR_MAX_SIZE) {
			top = p[i];
			for(int j = i+1; j < 4; j++) {
				if(p[j]->x >= top->x - IR_DIFF && p[j]->x <= top->x + IR_DIFF && p[j]->size < IR_MAX_SIZE) {
					bottom = p[j];
					break;
				}
			}
		}
	}

	if(top == NULL || bottom == NULL) {
		return;
	}

	if(bottom->y < top->y) {
		ir_point_t *temp = top;
		top = bottom;
		bottom = temp;
	}

	int diff = (bottom->y - top->y);
	int dist = s(diff);



	sprintf(debug, "\r\nHoriz: %4d, Diff: %4d, Dist: %4d (%4d,%4d,%2d-%4d,%4d,%2d)\r\n\r\n",
			top->x, diff, dist,
			top->x, top->y, top->size,
			bottom->x, bottom->y, bottom->size);
	//bt_puts(debug);

	int control_t = 0;
	if(dist > 300 && dist < 600) {
		control_t = (dist - 300)*30/300;
		if(control_t > 30)
			control_t = 30;
	}
	car_throttle(control_t);

	int control_s = (512 - (int) top->x)*100/512;
	car_steer(control_s);
	sprintf(debug, "H: %4d, D: %4d, Throttle: %4d, Steer: %4d\r\n", top->x, dist, control_t, control_s);
	bt_puts(debug);
}
