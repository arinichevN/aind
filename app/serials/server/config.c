
#include "../../../lib/acp/command/main.h"
#include "../../../model/Channel/main.h"
#include "../../channels/main.h"
#include "../../main.h"
#include "../main.h"

AcplsCommandNode *srvc_getServerCommandNext(int prev_command);
AcplsCommandNode *srvc_getServerCommand(int command);
int srvc_getServerCommandCount();

Channel *srvc_getChannel(Acpls *item){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		FOREACH_CHANNEL{
			if(id == channel->id){
				return channel;
			}
		}
		printdln("\tchannel not found");
		return NULL;

	}
	printdln("\tfailed to get channel_id");
	return NULL;
}

Channel *srvc_getServerChannel(Acpls *item){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		FOREACH_CHANNEL{
			if(id == channel->id && channel->mode == SERIAL_MODE_SERVER){
				return channel;
			}
		}
		printdln("\tchannel not found");
		return NULL;

	}
	printdln("\tfailed to get channel_id");
	return NULL;
}

Channel *srvc_getAppChannelNext(Acpls *item){
	int id;
	int found = 0;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &id)){
		FOREACH_CHANNEL{
			if(found) return channel;
			if(id == channel->id){
				found = 1;
				continue;
			}
		}
		printdln("\tchannel not found");
		return NULL;
	}
	printdln("\tfailed to get channel_id");
	return NULL;
}

int srvc_forThisApp(Acpls *item){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		if(id == app_id){
			return 1;
		}else{
			printdln("\tnot for this app");
			return 0;
		}
	}
	printdln("\tfailed to get app_id");
	return 0;
}

void srvc_setChannelParamUl(Acpls *item, void (*func)(ChannelParam *, unsigned long )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	unsigned long v;
	if(acp_packGetCellUl(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getChannelParam(&param, channel->ind)){
			func(&param, v);
			pmem_saveChannelParam(&param, channel->ind);
		}
	}
	acpls_reset(item);
}

void srvc_setChannelParamI(Acpls *item, void (*func)(ChannelParam *, int )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getChannelParam(&param, channel->ind)){
			func(&param, v);
			pmem_saveChannelParam(&param, channel->ind);
		}
	}
	acpls_reset(item);
}

void srvc_setChannelParamF(Acpls *item, void (*func)(ChannelParam *, double )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	double v;
	if(acp_packGetCellF(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getChannelParam(&param, channel->ind)){
			func(&param, v);
			pmem_saveChannelParam(&param, channel->ind);
		}
	}
	acpls_reset(item);
}

void fgetChannelPmemParamUl(Acpls *item, unsigned long (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	ChannelParam param;
	int f = 0;unsigned long out = 0;
	if(pmem_getChannelParam(&param, channel->ind)){
		out = getfunc(&param);
		f = 1;
	}
	int r = acp_buildPackIUlI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out, f);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void fgetChannelPmemParamI(Acpls *item, int (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	ChannelParam param;
	int f = 0;int out = 0;
	if(pmem_getChannelParam(&param, channel->ind)){
		out = getfunc(&param);
		f = 1;
	}
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out, f);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}
void fgetChannelPmemParamF(Acpls *item, double (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	ChannelParam param;
	int f = 0;double out = 0;
	if(pmem_getChannelParam(&param, channel->ind)){
		out = getfunc(&param);
		f = 1;
	}
	int r = acp_buildPackIFI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out, f);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void fgetAppPmemParamI(Acpls *item, int app_id, int (*getfunc)(AppParam *)){
	AppParam pdata;
	int f = 0;int out = 0;
	if(pmem_getAppParam(&pdata)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, out, f);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getChannelParamUl(Acpls *item, unsigned long (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	unsigned long out = getfunc(channel);
	int r = acp_buildPackIUl(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getChannelParamI(Acpls *item, int (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	int out = getfunc(channel);
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getConfirmChannelParamI(Acpls *item, int (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	int out = getfunc(channel);
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out, 1);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getChannelParamF(Acpls *item, double (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	double out = getfunc(channel);
	int r = acp_buildPackIF(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getChannelParamStr(Acpls *item, const char *(*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	const char *out = getfunc(channel);
	int r = acp_buildPackIS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_setId(Acpls *item, HardwareSerial *serial){srvc_setChannelParamI(item, &PMEMCHANNEL_SET_FIELD_FUNC(id));}

#define SC_FUN_SET(param) acnf_set__ ## param
#define SC_DEF_FUN_SET_UL(param) void SC_FUN_SET(param) (Acpls *item, HardwareSerial *serial){srvc_setChannelParamUl(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}
#define SC_DEF_FUN_SET_I(param) void SC_FUN_SET(param) (Acpls *item, HardwareSerial *serial){srvc_setChannelParamI(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}
#define SC_DEF_FUN_SET_F(param) void SC_FUN_SET(param) (Acpls *item, HardwareSerial *serial){srvc_setChannelParamF(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}

#define SC_FUN_GET(param) acnf_get__ ## param
#define SC_DEF_FUN_GET_UL(param) void SC_FUN_GET(param) (Acpls *item, HardwareSerial *serial){fgetChannelPmemParamUl(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}
#define SC_DEF_FUN_GET_I(param) void SC_FUN_GET(param) (Acpls *item, HardwareSerial *serial){fgetChannelPmemParamI(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}
#define SC_DEF_FUN_GET_F(param) void SC_FUN_GET(param) (Acpls *item, HardwareSerial *serial){fgetChannelPmemParamF(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}

#define SC_FUN_GETR(param) acnf_getr__ ## param
#define SC_DEF_FUN_GETR_UL(param) void SC_FUN_GETR(param) (Acpls *item, HardwareSerial *serial){srvc_getChannelParamUl(item, &CHANNEL_FUN_GET(param));}
#define SC_DEF_FUN_GETR_I(param) void SC_FUN_GETR(param) (Acpls *item, HardwareSerial *serial){srvc_getChannelParamI(item, &CHANNEL_FUN_GET(param));}
#define SC_DEF_FUN_GETR_F(param) void SC_FUN_GETR(param)(Acpls *item, HardwareSerial *serial){srvc_getChannelParamF(item, &CHANNEL_FUN_GET(param));}

#define SC_FUN_GETRC(param) acnf_getrc__ ## param
#define SC_DEF_FUN_GETRC_I(param) void SC_FUN_GETRC(param) (Acpls *item, HardwareSerial *serial){srvc_getConfirmChannelParamI(item, &CHANNEL_FUN_GET(param));}



SC_DEF_FUN_SET_I(display_kind)
SC_DEF_FUN_SET_I(display_p1)
SC_DEF_FUN_SET_I(display_p2)
SC_DEF_FUN_SET_I(display_p3)
SC_DEF_FUN_SET_I(display_text_alignment)
SC_DEF_FUN_SET_I(serial_id)
SC_DEF_FUN_SET_I(mode)
SC_DEF_FUN_SET_I(remote_id)
SC_DEF_FUN_SET_I(acp_command)
SC_DEF_FUN_SET_UL(time)
SC_DEF_FUN_SET_I(float_precision)

SC_DEF_FUN_GETRC_I(device_kind)

SC_DEF_FUN_GET_I(display_kind)
SC_DEF_FUN_GET_I(display_p1)
SC_DEF_FUN_GET_I(display_p2)
SC_DEF_FUN_GET_I(display_p3)
SC_DEF_FUN_GET_I(display_text_alignment)
SC_DEF_FUN_GET_I(serial_id)
SC_DEF_FUN_GET_I(mode)
SC_DEF_FUN_GET_I(remote_id)
SC_DEF_FUN_GET_I(acp_command)
SC_DEF_FUN_GET_UL(time)
SC_DEF_FUN_GET_I(float_precision)

SC_DEF_FUN_GETR_I(display_kind)
SC_DEF_FUN_GETR_I(display_p1)
SC_DEF_FUN_GETR_I(display_p2)
SC_DEF_FUN_GETR_I(display_p3)
SC_DEF_FUN_GETR_I(display_text_alignment)
SC_DEF_FUN_GETR_I(serial_id)
SC_DEF_FUN_GETR_I(mode)
SC_DEF_FUN_GETR_I(remote_id)
SC_DEF_FUN_GETR_I(acp_command)
SC_DEF_FUN_GETR_UL(time)
SC_DEF_FUN_GETR_I(float_precision)

SC_DEF_FUN_GETR_I(device_kind)
SC_DEF_FUN_GETR_I(enable)


void acnf_getErrorStr(Acpls *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getErrorStr);}
void acnf_getStateStr(Acpls *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getStateStr);}

void acnf_getExists(Acpls *item, HardwareSerial *serial){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		FOREACH_CHANNEL{
			if(id == channel->id){
				goto success;
			}
		}
		if(id == app_id)	goto success;
	}
	printd("id not found: "); printdln(id);
	acpls_reset(item);
	return;
	success:
	if(acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, 1)){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_getFTS (Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	FTS fts;
	fts.value = 0;
	fts.state = 1;
	fts.tm = getCurrentTs();
	if(acp_buildPackIFTS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, &fts)){
		acpls_prepWrite(item);
	}else{
		acpls_reset(item);
	}
}

void acnf_setIdFirst(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		FOREACH_CHANNEL{
			ChannelParam param;
			if(pmem_getChannelParam(&param, channel->ind)){
				param.id = v; v++;
				pmem_saveChannelParam(&param, channel->ind);
			}
		}
	}
	acpls_reset(item);
}

void acnf_setText(Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getServerChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	char v[CHANNEL_DISPLAY_ROWS];
	if(acp_packGetCellS(item->acpl->buf, ACP_REQUEST_IND_PARAM1, v, CHANNEL_DISPLAY_ROWS)){
		channel_serverPrint(channel, v);
	}
	acpls_reset(item);
}

void acnf_setTextBlink(Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getServerChannel(item);
	if(channel == NULL) {acpls_reset(item); return;}
	char v[CHANNEL_DISPLAY_ROWS];
	if(acp_packGetCellS(item->acpl->buf, ACP_REQUEST_IND_PARAM1, v, CHANNEL_DISPLAY_ROWS)){
		channel_serverPrintBlink(channel, v);
	}
	acpls_reset(item);
}

void acnf_start(Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_start(channel);
	}
	acpls_reset(item);
}

void acnf_stop(Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_stop(channel);
	}
	acpls_reset(item);
}

void acnf_reset(Acpls *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_reset(channel);
	}
	acpls_reset(item);
}

void acnf_getAppChannelCount (Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	if(acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, channels.length)){
		acpls_prepWrite(item);
	}else{
		acpls_reset(item);
	}
}

void acnf_getChannelIdFirst(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int out;
	int success = channelLList_getIdFirst(&channels, &out);
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, out, success);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_getChannelIdNext (Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	Channel *channel = srvc_getAppChannelNext(item);
	int success = 0;
	int id = 0;
	if(channel != NULL) {
		success = 1;
		id = channel->id;
	}
	if(acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, id, success)){
		acpls_prepWrite(item);
	}else{
		acpls_reset(item);
	}
}

void acnf_getAppAcpCommandExists(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int command;
	int success = 0;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &command)){
		goto done;
	}
	{AcplsCommandNode *node = srvc_getServerCommand(command);
	if(node != NULL) {
		success = 1;
	}}
	done:
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, command, success);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_getAppServerCommandCount(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int c = srvc_getServerCommandCount();
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app_id, c);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_getAppServerCommandFirst(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	extern AcplsCommandNodeList server_nodes;
	if(server_nodes.length < 1) {acpls_reset(item); return;}
	int first_command = server_nodes.items[0].command;
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app_id, first_command);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void acnf_getAppServerCommandNext(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int prev_command;
	int next_command = 0;
	int success = 0;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &prev_command)){
		goto done;
	}
	{AcplsCommandNode *node = srvc_getServerCommandNext(prev_command);
	if(node != NULL){
		success = 1;
		next_command = node->command;
	}}
	done:
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, next_command, success);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getrAppFieldSF(Acpls *item, const char * (*getfunc)()){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	const char *v = getfunc();
	int r = acp_buildPackIS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app_id, v);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getAppParamField(Acpls *item, int (*getfunc)(AppParam *)){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	AppParam conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppParam(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 v, result);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_getAppParamFieldBr(Acpls *item, int (*getfunc)(AppParam *)){
	AppParam conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppParam(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 v, result);
	if(r){acpls_prepWrite(item);}else{acpls_reset(item);}
}

void srvc_setAppParamField(Acpls *item, int (*checkfunc)(int ), void (*setfunc)(AppParam *, int v)){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		AppParam conf;
		if(pmem_getAppParam(&conf)){
			if(checkfunc(v)){
				setfunc(&conf, v);
				pmem_saveAppParam(&conf);
			}
		}
	}
	acpls_reset(item);
}

void srvc_setAppSerialParamFieldI(Acpls *item, int (*checkfunc)(int ), void (*setfunc)(AppSerialParam *, int v)){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int serial_id;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &serial_id)) {acpls_reset(item); return;}
	int v;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM2, &v)) {acpls_reset(item); return;}
	if(!checkfunc(v)) {acpls_reset(item); return;}
	size_t serial_ind = serials_getIndById(serial_id);
	if(serial_ind == -1) {acpls_reset(item); return;}
	AppSerialParam param;
	if(!pmem_getSerialParam(&param, serial_ind)) {acpls_reset(item); return;}
	setfunc(&param, v);
	pmem_saveSerialParam(&param, serial_ind);
}

void srvc_getAppParamSrFieldI(Acpls *item, int (*getfunc)(AppSerialParam *)){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	int serial_id;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &serial_id)) {acpls_reset(item); return;}
	size_t serial_ind = serials_getIndById(serial_id);
	int v = 0;
	int result = 0;
	AppSerialParam param;
	if(serial_ind == -1) goto done;
	if(!pmem_getSerialParam(&param, serial_ind)) goto done;
	v = getfunc(&param);
	result = 1;
	done:
	if(acp_buildPackIIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, serial_id, v, result)){
		acpls_prepWrite(item);
		return;
	}
	failed:
	acpls_reset(item);
}

void acnf_getAppSerialRate(Acpls *item, HardwareSerial *serial){srvc_getAppParamSrFieldI(item, &APP_SERIAL_PARAM_GET_FIELD_FUNC_I(rate));}
void acnf_getAppSerialParam(Acpls *item, HardwareSerial *serial){srvc_getAppParamSrFieldI(item, &APP_SERIAL_PARAM_GET_FIELD_FUNC_I(config));}
void acnf_getAppSerialMode(Acpls *item, HardwareSerial *serial){srvc_getAppParamSrFieldI(item, &APP_SERIAL_PARAM_GET_FIELD_FUNC_I(mode));}

void acnf_getAppId(Acpls *item, HardwareSerial *serial){srvc_getAppParamFieldBr(item, &APP_PARAM_GET_FIELD_FUNC(id));}
void acnf_getAppState(Acpls *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getStateStr);}
void acnf_getAppError(Acpls *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getErrorStr);}

void acnf_setAppId(Acpls *item, HardwareSerial *serial){
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &v)){
		AppParam param;
		if(pmem_getAppParam(&param)){
			param.id = v;
			if(appParam_check(&param)){
				pmem_saveAppParam(&param);
				if(pmem_getAppParam(&param)){
					app_id = param.id;
				}
			}
		}
	}
	acpls_reset(item);
}

void acnf_appReset(Acpls *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {acpls_reset(item); return;}
	app_reset();
	acpls_reset(item);
}

void acnf_setAppSerialConfig(Acpls *item, HardwareSerial *serial){srvc_setAppSerialParamFieldI(item, &serial_checkConfig, &APP_SERIAL_PARAM_SET_FIELD_FUNC_I(config));}
void acnf_setAppSerialRate(Acpls *item, HardwareSerial *serial){srvc_setAppSerialParamFieldI(item, &serial_checkRate, &APP_SERIAL_PARAM_SET_FIELD_FUNC_I(rate));}
void acnf_setAppSerialMode(Acpls *item, HardwareSerial *serial){srvc_setAppSerialParamFieldI(item, &serial_checkMode, &APP_SERIAL_PARAM_SET_FIELD_FUNC_I(mode));}

#define SS_GETR		SERIAL_SERVER_RAM_GET_COMMANDS
#define SS_GET		SERIAL_SERVER_NVRAM_GET_COMMANDS
#define SS_SET		SERIAL_SERVER_NVRAM_SET_COMMANDS

AcplsCommandNode acnodes[] = {

	{CMD_SET_INDICATOR_TEXT,					&acnf_setText},
	{CMD_SET_INDICATOR_TEXT_BLINK,				&acnf_setTextBlink},
	
#ifdef SS_GETR
	{CMD_GETR_CHANNEL_FTS,						&acnf_getFTS},
	{CMD_GETR_CHANNEL_STATE,					&acnf_getStateStr},
	{CMD_GETR_CHANNEL_ERROR,					&acnf_getErrorStr},
#endif

	{CMD_GET_ID_EXISTS,							&acnf_getExists},
	
	{CMD_GET_APP_CHANNEL_COUNT,					&acnf_getAppChannelCount},
	{CMD_GET_APP_ACP_COMMAND_EXISTS,			&acnf_getAppAcpCommandExists},
	{CMD_GET_APP_CHANNEL_ID_FIRST,				&acnf_getChannelIdFirst},
	{CMD_GET_APP_CHANNEL_ID_NEXT,				&acnf_getChannelIdNext},
	
	{CMD_CHANNEL_START,							&acnf_start},
	{CMD_CHANNEL_STOP,							&acnf_stop},
	{CMD_CHANNEL_RESET,							&acnf_reset},

#ifdef SS_SET
	{CMD_SET_INDICATOR_DISPLAY_KIND,			&SC_FUN_SET(display_kind)},
	{CMD_SET_INDICATOR_DISPLAY_P1,				&SC_FUN_SET(display_p1)},
	{CMD_SET_INDICATOR_DISPLAY_P2,				&SC_FUN_SET(display_p2)},
	{CMD_SET_INDICATOR_DISPLAY_P3,				&SC_FUN_SET(display_p3)},
	{CMD_SET_INDICATOR_TEXT_ALIGNMENT,			&SC_FUN_SET(display_text_alignment)},
	{CMD_SET_INDICATOR_SERIAL_ID,				&SC_FUN_SET(serial_id)},
	{CMD_SET_INDICATOR_MODE,					&SC_FUN_SET(mode)},
	{CMD_SET_INDICATOR_REMOTE_ID,				&SC_FUN_SET(remote_id)},
	{CMD_SET_INDICATOR_ACP_COMMAND,				&SC_FUN_SET(acp_command)},
	{CMD_SET_INDICATOR_TIME,					&SC_FUN_SET(time)},
	{CMD_SET_INDICATOR_FLOAT_PRECISION,			&SC_FUN_SET(float_precision)},
#endif

#ifdef SS_GET
	{CMD_GET_INDICATOR_DISPLAY_KIND,			&SC_FUN_GET(display_kind)},
	{CMD_GET_INDICATOR_DISPLAY_P1,				&SC_FUN_GET(display_p1)},
	{CMD_GET_INDICATOR_DISPLAY_P2,				&SC_FUN_GET(display_p2)},
	{CMD_GET_INDICATOR_DISPLAY_P3,				&SC_FUN_GET(display_p3)},
	{CMD_GET_INDICATOR_TEXT_ALIGNMENT,			&SC_FUN_GET(display_text_alignment)},
	{CMD_GET_INDICATOR_SERIAL_ID,				&SC_FUN_GET(serial_id)},
	{CMD_GET_INDICATOR_MODE,					&SC_FUN_GET(mode)},
	{CMD_GET_INDICATOR_REMOTE_ID,				&SC_FUN_GET(remote_id)},
	{CMD_GET_INDICATOR_ACP_COMMAND,				&SC_FUN_GET(acp_command)},
	{CMD_GET_INDICATOR_TIME,					&SC_FUN_GET(time)},
	{CMD_GET_INDICATOR_FLOAT_PRECISION,			&SC_FUN_GET(float_precision)},
#endif

#ifdef SS_GETR
	{CMD_GETR_INDICATOR_DISPLAY_KIND,			&SC_FUN_GETR(display_kind)},
	{CMD_GETR_INDICATOR_DISPLAY_P1,				&SC_FUN_GETR(display_p1)},
	{CMD_GETR_INDICATOR_DISPLAY_P2,				&SC_FUN_GETR(display_p2)},
	{CMD_GETR_INDICATOR_DISPLAY_P3,				&SC_FUN_GETR(display_p3)},
	{CMD_GETR_INDICATOR_TEXT_ALIGNMENT,			&SC_FUN_GETR(display_text_alignment)},
	{CMD_GETR_INDICATOR_SERIAL_ID,				&SC_FUN_GETR(serial_id)},
	{CMD_GETR_INDICATOR_MODE,					&SC_FUN_GETR(mode)},
	{CMD_GETR_INDICATOR_REMOTE_ID,				&SC_FUN_GETR(remote_id)},
	{CMD_GETR_INDICATOR_ACP_COMMAND,			&SC_FUN_GETR(acp_command)},
	{CMD_GETR_INDICATOR_TIME,					&SC_FUN_GETR(time)},
	{CMD_GETR_INDICATOR_FLOAT_PRECISION,		&SC_FUN_GETR(float_precision)},
#endif

#ifdef SS_GETR
	{CMD_GET_CHANNEL_DEVICE_KIND,				&SC_FUN_GETRC(device_kind)},
	{CMD_GETR_CHANNEL_DEVICE_KIND,				&SC_FUN_GETR(device_kind)},
	{CMD_GETR_CHANNEL_ENABLE,					&SC_FUN_GETR(enable)},
#endif

#ifdef SS_GETR
	{CMD_GET_CHANNEL_DEVICE_KIND,				&SC_FUN_GETR(device_kind)},
	{CMD_GETR_CHANNEL_DEVICE_KIND,				&SC_FUN_GETR(device_kind)},
	{CMD_GETR_CHANNEL_ENABLE,					&SC_FUN_GETR(enable)},
#endif

#ifdef SS_SET
	{CMD_SET_CHANNEL_ID,						&acnf_setId},
#endif

#ifdef SS_SET
	{CMD_SETB_APP_ID,							&acnf_setAppId},
	{CMD_SET_APP_CHANNEL_ID_FIRST,				&acnf_setIdFirst},
#endif

	{CMD_APP_RESET,								&acnf_appReset},
	
#ifdef SS_GETR
	{CMD_GETB_APP_ID,							&acnf_getAppId},
	{CMD_GET_APP_STATE,							&acnf_getAppState},
	{CMD_GET_APP_ERROR,							&acnf_getAppError},
	{CMD_GET_APP_SERVER_COMMAND_NEXT,			&acnf_getAppServerCommandNext},
	{CMD_GET_APP_SERVER_COMMAND_COUNT,			&acnf_getAppServerCommandCount},
	{CMD_GET_APP_SERVER_COMMAND_FIRST,			&acnf_getAppServerCommandFirst},
#endif

#ifdef SS_SET
	{CMD_SET_APP_SERIAL_MODE,					&acnf_setAppSerialMode},
	{CMD_SET_APP_SERIAL_RATE,					&acnf_setAppSerialRate},
	{CMD_SET_APP_SERIAL_CONFIG,					&acnf_setAppSerialConfig},
#endif

#ifdef SS_GET
	{CMD_GET_APP_SERIAL_RATE,					&acnf_getAppSerialRate},
	{CMD_GET_APP_SERIAL_CONFIG,					&acnf_getAppSerialParam},
	{CMD_GET_APP_SERIAL_MODE,					&acnf_getAppSerialMode}
#endif

};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])

AcplsCommandNodeList server_nodes = {.items = acnodes, .length = ACPL_CNODE_COUNT, .max_length = ACPL_CNODE_COUNT};

int srvc_getServerCommandCount(){
	return ACPL_CNODE_COUNT;
}

AcplsCommandNode *srvc_getServerCommandNext(int prev_command){
	int found = 0;
	for(size_t i = 0; i<server_nodes.length; i++){
		AcplsCommandNode *cnode = &server_nodes.items[i];
		if(found) return cnode;
		if(cnode->command == prev_command){
			found = 1;
			continue; 
		}
	}
	return NULL;
}

AcplsCommandNode *srvc_getServerCommand(int command){
	for(size_t i = 0; i<server_nodes.length; i++){
		AcplsCommandNode *cnode = &server_nodes.items[i];
		if(cnode->command == command){
			return cnode;
		}
	}
	return NULL;
}

