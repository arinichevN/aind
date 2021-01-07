#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

#include "../../lib/serial.h"
#include "../../lib/acp/command/main.h"
#include "../Display/common.h"

typedef struct {
	int id;
	int enable;
	int display_kind;
	int display_p1;
	int display_p2;
	int display_p3;
	int display_text_alignment;
	int serial_id;
	int mode;
	int remote_id;
	int acp_command;
	unsigned long time;
	int float_precision;
} ChannelParam;

extern int channelParam_check(const ChannelParam *self);

#endif 
