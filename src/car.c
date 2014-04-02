#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <car.h>

#define CAR_STEER		((PWMSP001_HandleType*)&PWMSP001_Handle1)
#define CAR_THROTTLE	((PWMSP001_HandleType*)&PWMSP001_Handle2)

static uint32_t steer_period;
static uint32_t steer_center;
static uint32_t steer_min;
static uint32_t steer_max;

static uint32_t throttle_period;
static uint32_t throttle_center;
static uint32_t throttle_min;
static uint32_t throttle_max;

int car_init(void)
{
	if(DAVEApp_SUCCESS != PWMSP001_GetPeriodReg(CAR_STEER, (uint32_t*) &steer_period)
			|| DAVEApp_SUCCESS != PWMSP001_GetPeriodReg(CAR_THROTTLE, (uint32_t*) &throttle_period)) {
		return 1;
	}

	/*
	 * Steering
	 *
	 * min		1,32 ms
	 * center	1,56 ms
	 * max		1,77 ms
	 */
	steer_min = 0.132f * (float) steer_period;
	steer_center = 0.156f * (float) steer_period;
	steer_max = 0.177f * (float) steer_period;

	/*
	 * Throttle
	 *
	 * min		1,32 ms
	 * center	1,56 ms
	 * max		1,77 ms
	 */
	throttle_min = 0.132f * (float) throttle_period;
	throttle_center = 0.156f * (float) throttle_period;
	throttle_max = 0.177f * (float) throttle_period;


	car_steer(0);
	car_throttle(0);

	return 0;
}

void car_steer(int percent)
{
	uint32_t compare = steer_center;

	// steer left
	if(percent < 0) {
		if(percent < -100)
			percent = -100;
		compare = steer_center + (percent * (steer_center - steer_min))/100;
	}

	// steer right
	if(percent > 0) {
		if(percent > 100)
			percent = 100;
		compare = steer_center + (percent * (steer_max - steer_center))/100;
	}

	(void) PWMSP001_SetCompare (CAR_STEER, compare);
}

void car_throttle(int percent)
{
	uint32_t compare = throttle_center;

	// steer left
	if(percent < 0) {
		if(percent < -100)
			percent = -100;
		compare = throttle_center + (percent * (throttle_center - throttle_min))/100;
	}

	// steer right
	if(percent > 0) {
		if(percent > 100)
			percent = 100;
		compare = throttle_center + (percent * (throttle_max - throttle_center))/100;
	}

	(void) PWMSP001_SetCompare (CAR_THROTTLE, compare);
}
