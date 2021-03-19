#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

#include "../../lib/common.h"
#include "../../lib/serial.h"
#include "../../lib/acp/command/main.h"
#include "../Display/common.h"
#include "common.h"

typedef struct {
	int id;
	yn_t enable;
	dk_t display_kind;
	int display_p1;
	int display_p2;
	int display_p3;
	talign_t display_text_alignment;
	int serial_id;
	channel_mode mode;
	int remote_id;
	int acp_command;
	unsigned long time;
	int float_precision;
} ChannelParam;

extern err_t channelParam_check(const ChannelParam *self);

#endif 
