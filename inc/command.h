/*
 * command.h
 *
 *  Created on: 18.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#ifndef COMMAND_H_
#define COMMAND_H_

enum command_item {
	I_HEADLIGHT,
	I_BRAKELIGHT,
	I_INDICATOR,
	I_THROTTLE,
	I_STEERING,
	I_PID_P,
	I_PID_I,
	I_PID_D,
	I_EMERGENCY,
	I_DEBUG,
	I_GO
};
enum command_dir {
	D_FWD, D_REV, D_LEFT, D_RIGHT, D_ALL
};

struct command {
	enum command_item item;
	enum command_dir dir;
	int value;
	float fvalue;
};


extern struct command * getCommand(void);

extern char * getItemStr(enum command_item);

extern char * getDirStr(enum command_dir);

#endif /* COMMAND_H_ */
