/*
 * command.c
 *
 *  Created on: 18.04.2014
 *      Author: Snafu
 */

#include <DAVE3.h>
#include <ctype.h>
#include <command.h>

#define COMMAND_HANDLE	UART001_Handle0
#define FIFO_SIZE		16

static char *itemStr[] = { "HEADLIGHT", "BRAKELIGHT", "INDICATOR", "THROTTLE",
		"STEERING", "EMERGENCY", "ACTIVE MODE" };
static char *dirStr[] = { "FWD", "REV", "LEFT", "RIGHT", "ALL" };

enum command_state {
	S_IDLE, S_DIR, S_SIDE, S_VALUE, S_WAITLINE
};

static struct command command_fifo[FIFO_SIZE];
static unsigned int fifo_write = 0;
static unsigned int fifo_read = 0;


void UART_handler(void);
static void addCommand(enum command_item item, enum command_dir dir, int value);
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

void addCommand(enum command_item item, enum command_dir dir, int value)
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
		} else {
			dir = D_LEFT;
			value *= -1;
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
 *
 *  All commands except E and G can be issued per as one command per line
 *  as multiple commands per line separated by a comma (",")
 *
 *  \param c		Character to parse
 */
static void commandParser(unsigned char c) {
	static enum command_state state = S_IDLE;
	static enum command_item item;
	static enum command_dir dir;
	static int value;

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
			if (isdigit(c)) {
				state = S_VALUE;
				value = c - '0';
				dir = D_ALL;
			} else {
				state = S_WAITLINE;
			}
			break;
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
		addCommand(item, dir, value);
	}
}

void UART_handler(void) {
	unsigned char c = (unsigned char) UART001_ReadData(COMMAND_HANDLE);
	commandParser(c);
}

