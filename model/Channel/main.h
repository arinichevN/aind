#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../lib/dstructure.h"
#include "../../lib/common.h"
#include "../../lib/ton.h"
#include "../../lib/acp/main.h"
#include "../../lib/acp/loop/main.h"
#include "../../lib/acp/loop/client/multi.h"
#include "../../lib/acp/loop/spy/main.h"
#include "../../lib/acp/command/main.h"
#include "../../app/serials/main.h"
#include "../../pmem/main.h"
#include "../Display/main.h"
#include "common.h"
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
	yn_t enable;
	
	talign_t display_text_alignment;
	
	int remote_id;
	unsigned long time;
	channel_mode mode;
	int acp_command;
	int serial_id;
	int float_precision;
	
	dk_t device_kind;
	int need_spy_response;
	Acplcm *serial_client;
	iAcplyClient im_spy_client;
	Display display;
	void (*parseNshowSpyData)(Channel *, char *buf);
	int (*parseNShowSpyRequestData)(Channel *, char *buf);
	const char *(*intToStr)(int);
	void (*control_next)(Channel *);
#ifdef USE_AOIDS
	ChannelAoid aoid;
#endif
#ifdef USE_NOIDS
	Noid noid;
#endif
	void (*control)(Channel *);
	err_t error_id;
	Ton tmr;
	size_t ind;
	Channel *next;
};

#define CHANNEL_SAVE_FIELD(F) ChannelParam pchannel; if(pmem_getChannelParam(&pchannel, self->ind)){pchannel.F = self->F; pmem_saveChannelParam(&pchannel, self->ind);}
#define CHANNEL_FUN_GET(param) channel_get_ ## param

extern state_t channel_getState(Channel *self);
extern err_t channel_getError(Channel *self);
extern void channel_begin(Channel *self, size_t ind);
extern int channel_start(Channel *self);
extern int channel_stop(Channel *self);
extern int channel_disconnect(Channel *self);
extern int channel_reset(Channel *self);
extern void channel_free(Channel *self);
extern void channel_serverPrint(Channel *self, const char *str);
extern void channel_serverPrintBlink(Channel *self, const char *str);
extern dk_t channel_getDisplayKind(Channel *self);
extern int channel_getDisplayP1(Channel *self);
extern int channel_getDisplayP2(Channel *self);
extern int channel_getDisplayP3(Channel *self);

#ifdef USE_AOIDS
extern void channel_buildAoids(Channel *self, Aoid *next_oid, Aoid *parent_oid, size_t *id);
#endif

#ifdef USE_NOIDS
extern Noid *channel_buildNoids(Channel *self);
#endif

#endif 
