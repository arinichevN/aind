#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../util/dstructure.h"
#include "../../util/common.h"
#include "../../util/ton.h"
#include "../../app/serial.h"
#include "../../acp/main.h"
#include "../../acp/loop/main.h"
#include "../../acp/loop/client/multi.h"
#include "../../acp/loop/spy.h"
#include "../display/main.h"
#include "../../pmem/main.h"
#include "param.h"

#define CHANNEL_DISPLAY_ROWS			16

#define CHANNEL_MSG_INIT				"ini"
#define CHANNEL_MSG_STOP				"stp"
#define CHANNEL_MSG_TIMEOUT				"Et"
#define CHANNEL_MSG_BAD_STATE			"ES"
#define CHANNEL_MSG_PARSE_FAILED		"EP"
#define CHANNEL_MSG_SPY_FAILED			"EY"
#define CHANNEL_MSG_CLIENT_FAILED		"EC"
#define CHANNEL_MSG_NOPARSENSHOW		"EPS"


#define CHANNEL_FLOAT_PRECISION_MAX		6

struct channel_st {
	int id;
	int enable;
	
	int display_text_alignment;
	
	int remote_id;
	unsigned long time;
	int mode;
	int acp_command;
	int serial_id;
	int float_precision;
	
	void (*control)(struct channel_st *);
	
	int device_kind;
	int need_spy_response;
	ACPLCM *acplcm;
	Display display;
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
#define CHANNEL_SAVE_FIELD(F) ChannelParam pchannel;	if(pmem_getPChannel(&pchannel, item->ind)){pchannel.F = item->F; pmem_savePChannel(&pchannel, item->ind);}
#define CHANNEL_FUN_GET(param) channel_get_ ## param

extern const char *channel_getStateStr(Channel *item);
extern const char *channel_getErrorStr(Channel *item);
extern void channel_begin(Channel *item, size_t ind);
extern void channels_begin(ChannelLList *channels);
extern int channels_getIdFirst(ChannelLList *channels, int *out);
extern int channels_coopSerials(ChannelLList *channels, AppSerial serials[]);
extern int channel_start(Channel *item);
extern int channel_stop(Channel *item);
extern int channel_reload(Channel *item);
extern void channel_free(Channel *item);
extern int channel_disconnect(Channel *item);
extern void channel_serverTouch(Channel *item);
extern int channels_activeExists(ChannelLList *channels);

extern int CHANNEL_FUN_GET(enable)(Channel *item);
extern int CHANNEL_FUN_GET(device_kind)(Channel *item);
extern int CHANNEL_FUN_GET(display_kind)(Channel *item);
extern int CHANNEL_FUN_GET(display_p1)(Channel *item);
extern int CHANNEL_FUN_GET(display_p2)(Channel *item);
extern int CHANNEL_FUN_GET(display_p3)(Channel *item);
extern int CHANNEL_FUN_GET(display_text_alignment)(Channel *item);
extern int CHANNEL_FUN_GET(serial_id)(Channel *item);
extern int CHANNEL_FUN_GET(mode)(Channel *item);
extern int CHANNEL_FUN_GET(remote_id)(Channel *item);
extern int CHANNEL_FUN_GET(acp_command)(Channel *item);
extern unsigned long CHANNEL_FUN_GET(time)(Channel *item);
extern int CHANNEL_FUN_GET(float_precision)(Channel *item);

#endif 
