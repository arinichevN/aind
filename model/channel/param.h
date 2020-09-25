#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

typedef struct {
	int id;
	int enable;
	int remote_id;
	int display_kind;
	int display_text_alignment;
	int display_p1;
	int display_p2;
	int display_p3;
	int mode;
	int acp_command;
	unsigned long time;
	int serial_id;
} ChannelParam;

#define CHANNEL_DEFAULT_PARAMS chdefps
#endif 
