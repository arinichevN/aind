#include "param.h"
#include "main.h"
#include "../../util/common.h"
#include "../../acp/command/main.h"

/*
 * -user_config:
 * put parameters for each channel in this array
 * ChannelParam structure members:
 * id,
 * enable: YES or NO,
 * display_kind: DEVICE_KIND_MAX7219 or DEVICE_KIND_TM1637 or DEVICE_KIND_DSERIAL,
 * display_p1: DIN pin for display_kind=DEVICE_KIND_MAX7219; DIO pin for display_kind=DEVICE_KIND_TM1637,
 * display_p2: CLK pin for display_kind=DEVICE_KIND_MAX7219; CLK pin for display_kind=DEVICE_KIND_TM1637,
 * display_p3: CS pin for display_kind=DEVICE_KIND_MAX7219,
 * display_text_alignment: DISPLAY_ALIGNMENT_LEFT or DISPLAY_ALIGNMENT_RIGHT,
 * serial_id: SERIAL_ID0 or SERIAL_ID1 or SERIAL_ID2 or SERIAL_ID3
 * mode: SERIAL_MODE_SPY or SERIAL_MODE_CLIENT or SERIAL_MODE_SERVER,
 * remote_id (for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_CLIENT),
 * command: CMD_GETR_CHANNEL_FTS or CMD_GETR_CHANNEL_STATE or CMD_GETR_CHANNEL_ERROR or CMD_SET_CHANNEL_GOAL, (set commands are only for mode=SERIAL_MODE_SPY, get commands are for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_CLIENT)
 * time: timeout (ms) for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_SERVER; poll interval (ms) for mode=SERIAL_MODE_CLIENT,
 * float_precision: number of digits to appear after decimal-point character in float variables
 */
const ChannelParam CHANNEL_DEFAULT_PARAMS[] = {
	{41, YES, DEVICE_KIND_TM1637, 4,3,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID1, SERIAL_MODE_SPY, 31, CMD_SET_CHANNEL_GOAL, 3000, 0},
	{42, YES, DEVICE_KIND_TM1637, 6,5,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID1, SERIAL_MODE_SPY, 32, CMD_SET_CHANNEL_GOAL, 3000, 0},
	{43, YES, DEVICE_KIND_TM1637, 8,7,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID1, SERIAL_MODE_SPY, 33, CMD_SET_CHANNEL_GOAL, 3000, 0}
};

#define CHANNEL_COUNT (sizeof CHANNEL_DEFAULT_PARAMS / sizeof CHANNEL_DEFAULT_PARAMS[0])
