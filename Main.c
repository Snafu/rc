/*
 * Main.c
 *
 *  Created on: 22.03.2014
 *      Author: Snafu
 */

#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <string.h>
#include <stdio.h>
#include <bluetooth.h>
#include <command.h>
#include <ircam.h>
#include <car.h>
#include <util.h>

#define CAR_INIT_ERROR	0x8f

#define PID_P		0.08f
#define PID_I		0.000f
#define PID_D		0.1f
#define SET_POS		300
#define MAX_SPEED	20
#define MIN_SPEED	0

#define IR_DIFF			50	// maximum ir_point_t x diff for matching set
#define IR_MAX_SIZE		3	// maximum ir_point_t size
#define IR_LED_DIST		80	// distance between LEDs in mm
#define IR_LED_POS_MIN	521	// ir_point_t y diff @ 200 mm
#define IR_LED_POS_SET	301 // ir_point_t y diff @ 300 mm
#define IR_LED_POS_MAX	153	// ir_point_t y diff @ 500 mm


char debug[100];
static volatile bool emergency_stop = FALSE;

static int s(int diff);
static void car_test(void);
static void car_control(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4);
static void execCommand(struct command *c);

int main(void) {
	DAVE_Init(); // Initialization of DAVE Apps
	debug_init();
	(void) car_init();
	bt_init();

	strcpy(debug, "Starting IR init\r\n");
	bt_puts(debug);
	ircam_init();

	ir_point_t p1, p2, p3, p4;
	strcpy(debug, "Polling data\r\n");
	bt_puts(debug);
	int counter = 0;

	struct command *c;
	while (1) {
		while((c = getCommand()) != NULL) {
			execCommand(c);
		}

		ircam_read(&p1, &p2, &p3, &p4);
		if(!emergency_stop) {
			car_control(&p1, &p2, &p3, &p4);
		}

		sprintf(debug, "#%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d-%4d,%4d,%2d\r\n",
				p1.x, p1.y, p1.size, p2.x, p2.y, p2.size, p3.x, p3.y, p3.size,
				p4.x, p4.y, p4.size);
		bt_puts(debug);

		if (counter++ == 2) {
			debug_knightrider();
			counter = 0;
		}
		wait(10);
	}
	return 0;
}

void car_test(void) {
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

int s(int diff) {
	return 100000 / diff;
}

void car_control(ir_point_t *p1, ir_point_t *p2, ir_point_t *p3, ir_point_t *p4) {
	ir_point_t *p[4] = { p1, p2, p3, p4 };
	ir_point_t *top = NULL;
	ir_point_t *bottom = NULL;

	static unsigned lock_lost_counter = 0;

	static int error_sum = 0;
	static int last_error = 0;
	int error;
	int error_diff;

	for (int i = 0; i < 4; i++) {
		if (bottom != NULL) {
			break;
		}
		if (p[i]->size < IR_MAX_SIZE) {
			top = p[i];
			for (int j = i + 1; j < 4; j++) {
				if (p[j]->x >= top->x - IR_DIFF && p[j]->x <= top->x + IR_DIFF
						&& p[j]->size < IR_MAX_SIZE) {
					bottom = p[j];
					break;
				}
			}
		}
	}

	if (top == NULL || bottom == NULL) {
		lock_lost_counter++;
		if(lock_lost_counter > 5) {
			bt_puts("Lost lock. Stopping car.\r\n");
			error_sum = 0;
			last_error = 0;
			car_throttle(0);
			car_steer(0);
		}
		return;
	}

	lock_lost_counter = 0;

	if (bottom->y < top->y) {
		ir_point_t *temp = top;
		top = bottom;
		bottom = temp;
	}

	int diff = (bottom->y - top->y);
	int dist = s(diff);

	sprintf(debug,
			"\r\nHoriz: %4d, Diff: %4d, Dist: %4d (%4d,%4d,%2d-%4d,%4d,%2d)\r\n\r\n",
			top->x, diff, dist, top->x, top->y, top->size, bottom->x, bottom->y,
			bottom->size);
	//bt_puts(debug);

	int control_t = 0;
	error = dist - SET_POS;
	error_diff = error - last_error;
	error_sum = error_sum + error;

	control_t = PID_P * (error + PID_I * error_sum + PID_D * error_diff);
	sprintf(debug, "DIST: %4d, C: %4d, E: %4d, S: %4d, L: %4d, D: %4d\r\n",
			dist, control_t, error, error_sum, last_error, error_diff);
	bt_puts(debug);

	if(control_t < MIN_SPEED) {
		control_t = MIN_SPEED;
	}
	if(control_t > MAX_SPEED) {
		control_t = MAX_SPEED;
	}
	car_throttle(control_t);
	last_error = error;

	/*
	if (dist > 300 && dist < 600) {
		control_t = (dist - 300) * 30 / 300;
		if (control_t > 30)
			control_t = 30;
	}
	car_throttle(control_t);
	*/

	int control_s = (512 - (int) top->x) * 100 / 512;
	car_steer(control_s);
}

static void execCommand(struct command *c) {
	switch (c->item) {
	case I_THROTTLE:
		if(!emergency_stop) {
			car_throttle(c->value);
		}
		break;

	case I_STEERING:
		if(!emergency_stop) {
			car_steer(c->value);
		}
		break;

	case I_EMERGENCY:
		emergency_stop = TRUE;
		car_throttle(0);
		car_steer(0);
		break;

	case I_GO:
		emergency_stop = FALSE;
		car_throttle(0);
		car_steer(0);
		break;

	default:
		break;
	}

	if (emergency_stop) {
		sprintf(debug, "\n\n!!! EMERGENCY STOP !!!\n\n");
		bt_puts(debug);
	} else {
		sprintf(debug, "* %s %s %d\n", getItemStr(c->item), getDirStr(c->dir), c->value);
		bt_puts(debug);
	}
}
