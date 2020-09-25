#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../util/dstructure.h"
#include "../../util/common.h"
#include "../../util/ton.h"
#include "../../app/serial.h"
#include "../../acp/main.h"
#include "../../acp/loop/main.h"
#include "../../acp/loop/client/multi.h"
#include "../display/main.h"
#include "param.h"

#define CHANNEL_DISPLAY_ROWS			16

#define CHANNEL_MSG_INIT				"ini"
#define CHANNEL_MSG_TIMEOUT				"Et"
#define CHANNEL_MSG_BAD_STATE			"ES"
#define CHANNEL_MSG_PARSE_FAILED		"EP"
#define CHANNEL_MSG_SPY_FAILED			"EY"
#define CHANNEL_MSG_CLIENT_FAILED		"EC"
#define CHANNEL_MSG_NOPARSENSHOW		"EPS"


#define CHANNEL_FLOAT_PRECISION			2

enum ChannelModeE {
	CHANNEL_MODE_SPY,
	CHANNEL_MODE_CLIENT
};

struct channel_st {
	int id;
	int enable;
	
	int display_text_alignment;
	
	int remote_id;
	unsigned long time;
	int mode; //spy or client
	int acp_command;
	int serial_id;
	
	void (*control)(struct channel_st *);
	
	int device_kind;
	int need_spy_response;
	ACPLCM *acplcm;
	Display display;
	void (*printFloat)(struct channel_st *, double);
	void (*parseNshowSpyData)(struct channel_st *, char *buf);
	int (*parseNShowSpyRequestData)(struct channel_st *, char *buf);
	void (*control_next)(struct channel_st *);
	int error_id;
	Ton tmr;
	size_t ind;
	struct channel_st *next;
};
typedef struct channel_st Channel;

DEC_LIST(Channel)
DEC_LLIST(Channel)

#define FOREACH_CHANNEL(LIST) FOREACH_LLIST(channel, LIST, Channel)

extern const char *channel_getStateStr(Channel *item);

extern const char *channel_getErrorStr(Channel *item);

extern void channel_begin(Channel *item, size_t ind);

extern void channels_begin(ChannelLList *channels);

extern int channels_coopSerials(ChannelLList *channels, AppSerial serials[]);

#endif 
