#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

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

#define CHANNEL_DEFAULT_PARAMS chdefps
#endif 
