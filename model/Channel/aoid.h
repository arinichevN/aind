#ifndef MODEL_CHANNEL_AOID_H
#define MODEL_CHANNEL_AOID_H

#include "../../lib/common.h"

typedef struct {
	Aoid main;
	Aoid id;
	Aoid enable;
	Aoid display_main;
	Aoid display_kind;
	Aoid display_p1;
	Aoid display_p2;
	Aoid display_p3;
	Aoid display_text_alignment;
	Aoid serial_id;
	Aoid mode;
	Aoid remote_id;
	Aoid acp_command;
	Aoid time;
	Aoid float_precision;
} ChannelAoid;

extern void channelAoid_build(ChannelAoid *self, Aoid *next, Aoid *parent, size_t *id, void *vchannel);

#endif 
