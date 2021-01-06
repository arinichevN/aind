#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../util/dstructure.h"
#include "../../util/common.h"
#include "../../util/ton.h"
#include "../../app/serials.h"
#include "../../acp/main.h"
#include "../../acp/loop/main.h"
#include "../../acp/loop/client/multi.h"
#include "../../acp/loop/spy/main.h"
#include "../../acp/command/main.h"
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

typedef struct channel_st Channel;
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
	
	void (*control)(Channel *);
	
	int device_kind;
	int need_spy_response;
	Acplcm *serial_client;
	iAcplyClient im_spy_client;
	Display display;
	void (*parseNshowSpyData)(Channel *, char *buf);
	int (*parseNShowSpyRequestData)(Channel *, char *buf);
	void (*control_next)(Channel *);
	int error_id;
	Ton tmr;
	size_t ind;
	Channel *next;
};

#define CHANNEL_SAVE_FIELD(F) ChannelParam pchannel; if(pmem_getChannelParam(&pchannel, self->ind)){pchannel.F = self->F; pmem_saveChannelParam(&pchannel, self->ind);}
#define CHANNEL_FUN_GET(param) channel_get_ ## param

extern const char *channel_getStateStr(Channel *self);
extern const char *channel_getErrorStr(Channel *self);
extern void channel_begin(Channel *self, size_t ind);
extern int channel_start(Channel *self);
extern int channel_stop(Channel *self);
extern int channel_disconnect(Channel *self);
extern int channel_reset(Channel *self);
extern void channel_free(Channel *self);
extern void channel_serverPrint(Channel *self, const char *str);
extern void channel_serverPrintBlink(Channel *self, const char *str);

extern int CHANNEL_FUN_GET(enable)(Channel *self);
extern int CHANNEL_FUN_GET(device_kind)(Channel *self);
extern int CHANNEL_FUN_GET(display_kind)(Channel *self);
extern int CHANNEL_FUN_GET(display_p1)(Channel *self);
extern int CHANNEL_FUN_GET(display_p2)(Channel *self);
extern int CHANNEL_FUN_GET(display_p3)(Channel *self);
extern int CHANNEL_FUN_GET(display_text_alignment)(Channel *self);
extern int CHANNEL_FUN_GET(serial_id)(Channel *self);
extern int CHANNEL_FUN_GET(mode)(Channel *self);
extern int CHANNEL_FUN_GET(remote_id)(Channel *self);
extern int CHANNEL_FUN_GET(acp_command)(Channel *self);
extern unsigned long CHANNEL_FUN_GET(time)(Channel *self);
extern int CHANNEL_FUN_GET(float_precision)(Channel *self);

#endif 
