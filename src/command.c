/*
 * command.c
 *
 *  Created on: 18.04.2014
 *      Author: Albin Frischenschlager, Christopher Gabriel, Matthias Zens
 */

#include <DAVE3.h>
#include <ctype.h>
#include <stdlib.h>
#include <command.h>

#define COMMAND_HANDLE	UART001_Handle0
#define FIFO_SIZE		16

static char *itemStr[] = { "HEADLIGHT", "BRAKELIGHT", "INDICATOR", "THROTTLE",
		"STEERING", "PID_P", "PID_I", "PID_D", "EMERGENCY", "DEBUG TOGGLE", "ACTIVE MODE" };
static char *dirStr[] = { "FWD", "REV", "LEFT", "RIGHT", "ALL" };

enum command_state {
	S_IDLE, S_DIR, S_SIDE, S_PID, S_FVALUE, S_VALUE, S_WAITLINE
};

static struct command command_fifo[FIFO_SIZE];
static unsigned int fifo_write = 0;
static unsigned int fifo_read = 0;


void UART_handler(void);
static void addCommand(enum command_item item, enum command_dir dir, int value, float fvalue);
static void commandParser(unsigned char c);

struct command * getCommand(void)
{
	struct command *ret = NULL;
	if(fifo_read != fifo_write) {
		ret = &command_fifo[fifo_read];
		fifo_read = (fifo_read + 1) % FIFO_SIZE;
	}

	return ret;
}

char * getItemStr(enum command_item item)
{
	return itemStr[item];
}

char * getDirStr(enum command_dir dir)
{
	return dirStr[dir];
}

void addCommand(enum command_item item, enum command_dir dir, int value, float fvalue)
{
	switch (item) {
	case I_THROTTLE:
		if (value > 100) {
			return;
		}
		if (dir == D_REV) {
			value *= -1;
		}
		break;

	case I_STEERING:
		if (value > 100) {
			return;
		}
		if (dir == D_FWD) {
			dir = D_RIGHT;
		} else if(dir == D_REV) {
			dir = D_LEFT;
			value *= -1;
		} else {
			value = 0;
		}
		break;

	case I_EMERGENCY:
		dir = D_ALL;
		value = 0;
		break;

	case I_GO:
		dir = D_ALL;
		value = 0;
		break;

	default:
		break;
	}
	command_fifo[fifo_write].item = item;
	command_fifo[fifo_write].dir = dir;
	command_fifo[fifo_write].value = value;
	command_fifo[fifo_write].fvalue = fvalue;

	fifo_write = (fifo_write + 1) % FIFO_SIZE;
}

/*! \brief Command parser
 *
 *  Possible commands are:
 *   E               ... emergency stop (sets everything to 0)
 *   G               ... Change to active mode (after emergency stop)
 *   T[+-][percent]  ... throttle (defaults: forward, 0%)
 *   S[+-][percent]  ... steering (defaults: right, 0%)
 *   H[LR][percent]  ... headlights (defaults: all, 0%)
 *   B[LR][percent]  ... breaklights (defaults: all, 0%)
 *   I[LR][percent]  ... indicators (defaults: all, 0%)
 *   _P float        ... PID_P value
 *   _I float        ... PID_I value
 *   _D float        ... PID_D value
 *
 *  All commands can be issued per as one command per line
 *  as multiple commands per line separated by a comma (",")
 *
 *  \param c		Character to parse
 */
static void commandParser(unsigned char c) {
	static enum command_state state = S_IDLE;
	static enum command_item item;
	static enum command_dir dir;
	static int value;
	static float fvalue;
	static int counter;
	static unsigned char buf[10];

	bool add_command = FALSE;

	if (c == '\r')
		return;

	c = toupper(c);

	switch (state) {
	case S_IDLE:
		switch (c) {
		case 'E':
			state = S_WAITLINE;
			item = I_EMERGENCY;
			add_command = TRUE;
			break;

		case 'G':
			state = S_WAITLINE;
			item = I_GO;
			add_command = TRUE;
			break;

		case 'T':
			state = S_DIR;
			item = I_THROTTLE;
			break;

		case 'S':
			state = S_DIR;
			item = I_STEERING;
			break;

		case 'H':
			state = S_SIDE;
			item = I_HEADLIGHT;
			break;

		case 'B':
			state = S_SIDE;
			item = I_BRAKELIGHT;
			break;

		case 'I':
			state = S_SIDE;
			item = I_INDICATOR;
			break;

		case '_':
			state = S_PID;
			break;

		case 'D':
			item = I_DEBUG;
			add_command = TRUE;
			state = S_WAITLINE;
			break;

		case ' ':
		case '\t':
		case '\n':
			// whitespace, remain in state
			break;

		default:
			// invalid character
			state = S_WAITLINE;
			break;
		}
		break;

	case S_DIR:
		dir = D_FWD;
		value = 0;
		if (isdigit(c)) {
			value = c - '0';
			state = S_VALUE;
		} else if (c == '-') {
			dir = D_REV;
			state = S_VALUE;
		} else if (c == '+') {
			state = S_VALUE;
		} else if (c == '\n') {
			state = S_IDLE;
			add_command = TRUE;
		} else {
			state = S_WAITLINE;
		}
		break;

	case S_SIDE:
		value = 0;
		switch (c) {
		case 'L':
			state = S_VALUE;
			dir = D_LEFT;
			break;

		case 'R':
			state = S_VALUE;
			dir = D_RIGHT;
			break;

		case ',':
		case '\n':
			state = S_IDLE;
			dir = D_ALL;
			add_command = TRUE;
			break;

		default:
			/*
			if (isdigit(c)) {
				state = S_VALUE;
				value = c - '0';
				dir = D_ALL;
			} else {
				state = S_WAITLINE;
			}
			*/
			state = S_WAITLINE;
			break;
		}
		break;

	case S_PID:
		fvalue = 0;
		state = S_FVALUE;
		counter = 0;
		switch(c) {
			case 'P':
				item = I_PID_P;
			break;

			case 'I':
				item = I_PID_I;
			break;

			case 'D':
				item = I_PID_D;
			break;

			default:
				state = S_WAITLINE;
			break;
		}
		break;

	case S_FVALUE:
		if(counter >= 10) {
			counter = 0;
			*buf = '\0';
			state = S_WAITLINE;
		} else if(isdigit(c) || c == '.') {
			buf[counter++] = c;
		}
		else if (c == ',' || c == '\n') {
			buf[counter] = '\0';
			char *eptr = NULL;
			fvalue = strtof((const char*) buf, &eptr);
			if(*eptr == '\0') {
				value = 0;
				state = S_IDLE;
				add_command = TRUE;
			} else {
				state = S_WAITLINE;
			}
		} else {
			state = S_WAITLINE;
		}
		break;

	case S_VALUE:
		if (isdigit(c)) {
			value *= 10;
			value += c - '0';
		} else if (c == ',' || c == '\n') {
			state = S_IDLE;
			add_command = TRUE;
		} else {
			state = S_WAITLINE;
		}
		break;

	case S_WAITLINE:
		if (c == '\n') {
			state = S_IDLE;
		}
		break;
	}

	if(add_command) {
		addCommand(item, dir, value, fvalue);
	}
}

void UART_handler(void) {
	unsigned char c = (unsigned char) UART001_ReadData(COMMAND_HANDLE);
	commandParser(c);
}

