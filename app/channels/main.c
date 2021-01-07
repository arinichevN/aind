#include "main.h"

/*
 * -user_config:
 * put parameters for each channel in this array
 * ChannelParam structure members:
 * id,
 * enable: YES or NO,
 * display_kind: DEVICE_KIND_MAX7219 or DEVICE_KIND_TM1637 or DEVICE_KIND_DSERIAL or DEVICE_KIND_DSLED (usefull only for command=CMD_GETR_CHANNEL_STATE),
 * display_p1: DIN pin for display_kind=DEVICE_KIND_MAX7219; DIO pin for display_kind=DEVICE_KIND_TM1637; run pin for display_kind=DEVICE_KIND_DSLED,
 * display_p2: CLK pin for display_kind=DEVICE_KIND_MAX7219; CLK pin for display_kind=DEVICE_KIND_TM1637; stop pin for display_kind=DEVICE_KIND_DSLED,
 * display_p3: CS pin for display_kind=DEVICE_KIND_MAX7219; failure pin for display_kind=DEVICE_KIND_DSLED,
 * display_text_alignment: DISPLAY_ALIGNMENT_LEFT or DISPLAY_ALIGNMENT_RIGHT,
 * serial_id: SERIAL_ID_0 or SERIAL_ID_1 or SERIAL_ID_2 or SERIAL_ID_3
 * mode: SERIAL_MODE_SPY or SERIAL_MODE_CLIENT or SERIAL_MODE_SERVER,
 * remote_id (for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_CLIENT),
 * command: CMD_GETR_CHANNEL_FTS or CMD_GETR_CHANNEL_STATE or CMD_GETR_CHANNEL_ERROR or CMD_SET_CHANNEL_GOAL, (set commands are only for mode=SERIAL_MODE_SPY, get commands are for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_CLIENT)
 * time: timeout (ms) for mode=SERIAL_MODE_SPY or mode=SERIAL_MODE_SERVER; poll interval (ms) for mode=SERIAL_MODE_CLIENT,
 * float_precision: number of digits to appear after decimal-point character in float variables
 */
const ChannelParam CHANNEL_DEFAULT_PARAMS[] = {
	//{21, YES, DEVICE_KIND_TM1637, 4,3,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_SERVER, 21, CMD_GETR_CHANNEL_FTS, 5000, 0},
	//{22, YES, DEVICE_KIND_TM1637, 6,5,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_SERVER, 22, CMD_GETR_CHANNEL_FTS, 5000, 0},
	//{23, YES, DEVICE_KIND_TM1637, 8,7,0, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_SERVER, 23, CMD_GETR_CHANNEL_FTS, 5000, 0}
	
	//{21, YES, DEVICE_KIND_MAX7219, 3,4,5, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 101, CMD_GETR_CHANNEL_FTS, 5000, 0},
	//{22, YES, DEVICE_KIND_MAX7219, 3,4,6, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 102, CMD_GETR_CHANNEL_FTS, 5000, 0},
	//{23, YES, DEVICE_KIND_MAX7219, 3,4,7, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 103, CMD_GETR_CHANNEL_FTS, 5000, 0}
	
	{21, YES, DEVICE_KIND_DSLED, 3,4,5, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 1, CMD_GETR_CHANNEL_STATE, 500, 0},
	{22, YES, DEVICE_KIND_DSLED, 6,7,8, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 2, CMD_GETR_CHANNEL_STATE, 500, 0},
	{23, YES, DEVICE_KIND_DSLED, 9,10,11, DISPLAY_ALIGNMENT_RIGHT, SERIAL_ID_1, SERIAL_MODE_CLIENT, 3, CMD_GETR_CHANNEL_STATE, 500, 0}
};

#define CHANNEL_COUNT (sizeof CHANNEL_DEFAULT_PARAMS / sizeof CHANNEL_DEFAULT_PARAMS[0])

Channel channel_buf[CHANNEL_COUNT];

ChannelLList channels = LLIST_INITIALIZER;

int channels_activeExists(){
	return channelLList_activeExists(&channels);
}

int channels_getIdFirst(int *out){
	return channelLList_getIdFirst(&channels, out);
}

void channels_begin(int default_btn){
	LLIST_BUILD_FROM_ARRAY_N(channels, channel_buf, CHANNEL_COUNT)
	size_t ind = 0;
	FOREACH_CHANNEL {
		if(default_btn == BUTTON_DOWN){
			pmem_saveChannelParam(&CHANNEL_DEFAULT_PARAMS[ind], ind);
			printd("default param has been saved for channel ind \n"); printdln(ind);
		}
		channel_begin(channel, ind); ind++;
	}
}

