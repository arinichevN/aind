#include "main.h"

static void channel_SERVER_FAILED(Channel *self);

static void channel_SERVER(Channel *self);

static void channel_SPY_FAILED(Channel *self);

static void channel_SPY(Channel *self);

static void channel_CLIENT_WAIT(Channel *self);

static void channel_CLIENT_FTS_GET(Channel *self);

static void channel_CLIENT_STR_GET(Channel *self);

static void channel_INIT(Channel *self);

static void channel_OFF(Channel *self);

static void channel_FAILURE(Channel *self);

static void channel_parseNshowSpyNothing(Channel *self, char *buf);

static void channel_parseNshowSpyFTS(Channel *self, char *buf);

static void channel_parseNshowSpyStr(Channel *self, char *buf);

static int channel_parseNShowSpyRequestIdle(Channel *self, char *buf);

static int channel_parseNShowSpyRequestSkip(Channel *self, char *buf);

static int channel_parseNShowSpyRequestFloat(Channel *self, char *buf);

static void channel_clearStr(Channel *self);

static void channel_printStr(Channel *self, const char *str);

static void channel_printStrBlink(Channel *self, const char *str);

static void channel_printError(Channel *self, const char *str);

static void channel_printFloat(Channel *self, double v);

static int channel_onSpyRequest(void *vself, char *buf, int id, int command);

static void channel_onSpyResponse(void *vself, char *buf, int id, int success);

const char *channel_getStateStr(Channel *self){
	if(self->control == channel_CLIENT_FTS_GET || self->control == channel_CLIENT_STR_GET || self->control == channel_CLIENT_WAIT || self->control == channel_SPY || self->control == channel_SERVER){ 
		return "RUN";
	} else if(self->control == channel_OFF) {
		return "OFF";
	} else if(self->control == channel_INIT) {
		return "INIT";
	}
	return "FAILURE";
}

const char *channel_getErrorStr(Channel *self){
	return getErrorStr(self->error_id);
}

static void channel_buildInterfaces(Channel *self){
	self->im_spy_client.self = self;
	self->im_spy_client.onRequestFunction = channel_onSpyRequest;
	self->im_spy_client.onResponseFunction = channel_onSpyResponse;
}

static int channel_setParam(Channel *self, size_t ind){
	ChannelParam param;
	if(!pmem_getChannelParam(&param, ind)){
		printdln("   failed to get channel from NVRAM");
		return ERROR_NVRAM_READ;
	}
	int r = channelParam_check(&param);
	if(r != ERROR_NO){
		return r;
	}
	self->remote_id = param.remote_id;
	if(!display_begin(&self->display, param.display_kind, param.display_p1, param.display_p2, param.display_p3)){
		printdln("display begin failed");
		return ERROR_SOME;
	}
	self->display_text_alignment = param.display_text_alignment;
	self->mode = param.mode;
	self->acp_command = param.acp_command;
	self->parseNshowSpyData = channel_parseNshowSpyNothing;
	self->parseNShowSpyRequestData = channel_parseNShowSpyRequestIdle;
	switch(self->acp_command){
		case CMD_GETR_CHANNEL_FTS:
			self->parseNshowSpyData = channel_parseNshowSpyFTS;
			self->parseNShowSpyRequestData = channel_parseNShowSpyRequestSkip;
			break;
		case  CMD_GETR_CHANNEL_STATE:
		case CMD_GETR_CHANNEL_ERROR:
			self->parseNshowSpyData = channel_parseNshowSpyStr;
			self->parseNShowSpyRequestData = channel_parseNShowSpyRequestSkip;
			break;
		case CMD_SET_CHANNEL_GOAL:
			self->parseNShowSpyRequestData = channel_parseNShowSpyRequestFloat;
			break;
	}
	ton_setInterval(&self->tmr, param.time);
	self->serial_id = param.serial_id;
	self->float_precision = param.float_precision;
	self->id = param.id;
	self->enable = param.enable;
	self->device_kind = DEVICE_KIND_INDICATOR;
	self->ind = ind;
	return ERROR_NO;
}

void channel_begin(Channel *self, size_t ind){
	printd("beginning channel ");printd(ind); printdln(":");
	channel_buildInterfaces(self);
	self->error_id = channel_setParam(self, ind);
	self->control = channel_INIT;
	printd("\tid: ");printdln(self->id);
	printd("\n");
}


void channel_free(Channel *self){
	display_free(&self->display);
}

int channel_start(Channel *self){
	if(self->control == channel_OFF || self->control == channel_FAILURE){
		printd("starting channel ");printd(self->ind);printdln(":");
		self->enable = YES;
		CHANNEL_SAVE_FIELD(enable)
		self->control = channel_INIT;
		return 1;
	}
	return 0;
}

int channel_stop(Channel *self){
	printd("stopping channel ");printdln(self->ind);
	channel_clearStr(self);
	self->error_id = ERROR_NO;
	self->enable = NO;
	CHANNEL_SAVE_FIELD(enable)
	self->control = channel_OFF;
	return 1;
}

int channel_disconnect(Channel *self){
	printd("disconnecting channel ");printdln(self->ind);
	channel_clearStr(self);
	self->error_id = ERROR_NO;
	self->control = channel_OFF;
	return 1;
}

int channel_reset(Channel *self){
	printd("restarting channel ");printd(self->ind); printdln(":");
	channel_free(self);
	channel_begin(self, self->ind);
	return 1;
}

static void channel_clearStr(Channel *self){
	DISPLAY_CLEARN(self->display);
}

static void channel_printStr(Channel *self, const char *str){
	DISPLAY_PRINT_STRN(self->display, str, self->display_text_alignment);
}

static void channel_printStrBlink(Channel *self, const char *str){
	DISPLAY_PRINT_BLINK_STRN(self->display, str, self->display_text_alignment);
}

static void channel_printError(Channel *self, const char *str){
	DISPLAY_PRINT_BLINK_STRN(self->display, str, self->display_text_alignment);
}

static void channel_printFloat(Channel *self, double v){
	char str[CHANNEL_DISPLAY_ROWS];
	dtostrf(v, 0, self->float_precision, str);
	DISPLAY_PRINT_STRN(self->display, str, self->display_text_alignment);
}

static void channel_parseNshowSpyNothing(Channel *self, char *buf){
	channel_printError(self, CHANNEL_MSG_NOPARSENSHOW);
	printd("channel "); printd(self->id); printdln(": no parseNshow function");
}

static void channel_parseNshowSpyFTS(Channel *self, char *buf){
	double v;
	if(!acp_packGetCellF (buf, ACP_RESPONSE_IND_PARAM1, &v)){
		channel_printError(self, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	int state;
	if(!acp_packGetCellI (buf, ACP_RESPONSE_IND_PARAM4, &state)){
		channel_printError(self, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	if(!state){
		channel_printError(self, CHANNEL_MSG_BAD_STATE);
		return;
	}
	channel_printFloat(self, v);
	printd(self->id); printd(" spy fts: "); printdln(v);
}

static void channel_parseNshowSpyStr(Channel *self, char *buf){
	char s[CHANNEL_DISPLAY_ROWS];
	memset(s, 0, sizeof s);
	if(!acp_packGetCellS (buf, ACP_RESPONSE_IND_PARAM1, s, CHANNEL_DISPLAY_ROWS)){
		channel_printError(self, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	channel_printStr(self, s);
	printd(self->id); printd(" spy str: "); printdln(s);
}

static int channel_parseNShowSpyRequestIdle(Channel *self, char *buf){
	int need_spy_response = 0;
	return need_spy_response;
}

static int channel_parseNShowSpyRequestSkip(Channel *self, char *buf){
	int need_spy_response = 1;
	return need_spy_response;
}

static int channel_parseNShowSpyRequestFloat(Channel *self, char *buf){
	double v;
	if(acp_packGetCellF(buf, ACP_REQUEST_IND_PARAM1, &v)){
		channel_printFloat(self, v);
		printd(self->id); printd(" spy float: "); printdln(v);
	}else{
		channel_printError(self, CHANNEL_MSG_PARSE_FAILED);
	}
	ton_reset(&self->tmr);
	self->control = channel_SPY;
	int need_spy_response = 0;
	return need_spy_response;
}

static int channel_onSpyRequest(void *vself, char *buf, int id, int command){
	printd("spy request: ");printdln(buf);
	Channel *self = (Channel *) vself;
	self->need_spy_response = 0;
	if(!(self->control == channel_SPY || self->control == channel_SPY_FAILED)){
		printd("spy request: "); printd(self->id); printdln(" bad channel state");
		return self->need_spy_response;
	}
	if(!self->enable){
		printd("spy request: "); printd(self->id); printdln("not active");
		return self->need_spy_response;
	}
	if(id != self->remote_id){
		printd("spy request: "); printd(self->id); printd(": bad id = ");  printd(id); printdln(" in request"); 
		return self->need_spy_response;
	}
	if(command != self->acp_command){
		printd("spy request: "); printd(self->id); printd(": bad command = ");  printd(command); printdln(" in request"); 
		return self->need_spy_response;
	}
	self->need_spy_response = self->parseNShowSpyRequestData(self, buf);
	printd("spy request: "); printd(self->id); printdln(" wanted request detected");
	return self->need_spy_response;
}

static void channel_onSpyResponse(void *vself, char *buf, int id, int success){
	printd("spy response: "); printdln(buf);
	Channel *self = (Channel *) vself;
	printd("spy response: "); printd(self->id); printd(" wanted response detected ");
	if(!(self->control == channel_SPY || self->control == channel_SPY_FAILED)){
		return;
	}
	if(!success){
		self->need_spy_response = 0;
		channel_printError(self, CHANNEL_MSG_SPY_FAILED);
		printdln("spy response: not success");
		return;
	}
	if(!self->need_spy_response) {
		//printd("spy response "); printd(self->id); printdln(" unwanted response"); 
		return;
	}
	if(id != self->remote_id){
		printd("spy response "); printd(self->id); printd(": bad id = ");  printd(id); printdln(" in response"); 
		return;
	}
	self->parseNshowSpyData(self, buf);
	ton_reset(&self->tmr);
	self->control = channel_SPY;
}


static void channel_setControlFunction(Channel *self){
	self->control = NULL;
	switch(self->mode){
		case SERIAL_MODE_SERVER:
			self->control = channel_SERVER; self->control_next = channel_FAILURE; return;
			break;
		case SERIAL_MODE_CLIENT:
			switch(self->acp_command){
				case CMD_GETR_CHANNEL_FTS:		self->control = channel_CLIENT_FTS_GET; self->control_next = channel_CLIENT_FTS_GET; return;
				case CMD_GETR_CHANNEL_STATE:	self->control = channel_CLIENT_STR_GET; self->control_next = channel_CLIENT_STR_GET; return;
				case CMD_GETR_CHANNEL_ERROR:	self->control = channel_CLIENT_STR_GET; self->control_next = channel_CLIENT_STR_GET; return;
			}
			break;
		case SERIAL_MODE_SPY:
			switch(self->acp_command){
				case CMD_GETR_CHANNEL_FTS:
				case CMD_GETR_CHANNEL_STATE:
				case CMD_GETR_CHANNEL_ERROR:
				case CMD_SET_CHANNEL_GOAL:
					self->control = channel_SPY;  self->control_next = channel_FAILURE; return;
			}
			break;
	}
}

static void channel_setSerial(Channel *self){
	switch(self->mode){
		case SERIAL_MODE_CLIENT:
			self->serial_client = serials_getClientById(self->serial_id);
			printd("serial "); printd(self->serial_id); printd(" client assigned to channel.id = "); printdln(self->id);
			if(self->serial_client == NULL){
				self->error_id = ERROR_NO_SERIAL;
				printd("bad serial: "); printd(self->serial_id); printdln(" client expected"); 
			}
			break;
		case SERIAL_MODE_SPY:{
			Acply *spy = serials_getSpyById(self->serial_id);
			if(spy != NULL){
				if(!acply_addClient(spy, &self->im_spy_client)){
					printd("add channel "); printd(self->id); printd(" to spy "); printdln(self->serial_id);
				}
			}else{
				self->error_id = ERROR_NO_SERIAL;
				printd("bad serial: "); printd(self->serial_id); printdln(" spy expected"); 
			}
			}
	}
}


static void channel_SERVER_FAILED(Channel *self){
	DISPLAY_CONTROLN(self->display);
}

static void channel_SERVER(Channel *self){
	if(ton(&self->tmr)){
		channel_printError(self, CHANNEL_MSG_TIMEOUT);
		self->control = channel_SERVER_FAILED;
		//printd(self->id); printdln(" server: timeout"); 
	}
	DISPLAY_CONTROLN(self->display);
}

static void channel_SPY_FAILED(Channel *self){
	DISPLAY_CONTROLN(self->display);
}

static void channel_SPY(Channel *self){
	if(ton(&self->tmr)){
		channel_printError(self, CHANNEL_MSG_TIMEOUT);
		self->control = channel_SPY_FAILED;
		//printd(self->id); printdln(" spy: timeout");
	}
	DISPLAY_CONTROLN(self->display);
}

static void channel_CLIENT_WAIT(Channel *self){
	if(ton(&self->tmr)){
		self->control = self->control_next;
	}
	DISPLAY_CONTROLN(self->display);
}

static void channel_CLIENT_FTS_GET(Channel *self){
	FTS out;
	int r = acplcm_getFTS(self->serial_client, self, self->acp_command, self->remote_id, &out);
	switch(r){
		case ACP_WAIT:
		case ACP_BUSY:
			break;
		case ACP_DONE:
			if(!out.state){
				channel_printError(self, CHANNEL_MSG_BAD_STATE);
				printd(self->id); printdln(" fts: bad state");
			}else{
				channel_printFloat(self, out.value);
				printd(self->id); printd(" fts: "); printdln(out.value);
			}
			printdln(" ");
			ton_reset(&self->tmr);
			self->control = channel_CLIENT_WAIT;
			break;
		default://error
			channel_printError(self, CHANNEL_MSG_CLIENT_FAILED);
			ton_reset(&self->tmr);
			self->control = channel_CLIENT_WAIT;
			printd(self->id); printd(" fts: failed: "); printd(acp_getStateStr(r)); printdln(" ");
			break;
	}
	DISPLAY_CONTROLN(self->display);
}

static void channel_CLIENT_STR_GET(Channel *self){
	char out[CHANNEL_DISPLAY_ROWS];
	int r = acplcm_getIS(self->serial_client, self, self->acp_command, self->remote_id, out, CHANNEL_DISPLAY_ROWS);
	switch(r){
		case ACP_WAIT:
		case ACP_BUSY:
			break;
		case ACP_DONE:
			channel_printStr(self, out);
			ton_reset(&self->tmr);
			self->control = channel_CLIENT_WAIT;
			printd(self->id); printd(" str: "); printdln(out);printdln(" ");
			break;
		default://error
			channel_printError(self, CHANNEL_MSG_CLIENT_FAILED);
			ton_reset(&self->tmr);
			self->control = channel_CLIENT_WAIT;
			printd(self->id); printd(" str: failed: "); printd(acp_getStateStr(r)); printdln(" ");
			break;
	}
	DISPLAY_CONTROLN(self->display);
}

static void channel_INIT(Channel *self){
	channel_setSerial(self);
	if(self->error_id != ERROR_NO){
		printd("channel_INIT "); printd(self->id); printdln(" error");
		self->control = channel_FAILURE;
		return;
	}
	channel_setControlFunction(self);
	if(self->control == NULL){
		printd("channel_INIT "); printd(self->id); printdln(" control function error");
		self->control = channel_FAILURE;
		return;
	}
	if(self->enable != YES){
		self->control = channel_OFF;
		return;
	}
	ton_reset(&self->tmr);
	ton_expire(&self->tmr);
	channel_printStr(self, CHANNEL_MSG_INIT);
}

static void channel_OFF(Channel *self){
	;
}

static void channel_FAILURE(Channel *self){
	;
}

#define CHANNEL_SERVER_PRINT_CONDITION self->mode == SERIAL_MODE_SERVER && (self->control == channel_SERVER || self->control == channel_SERVER_FAILED)

void channel_serverPrint(Channel *self, const char *str){
	if(CHANNEL_SERVER_PRINT_CONDITION){
		ton_reset(&self->tmr);
		channel_printStr(self, str);
		self->control = channel_SERVER;
	}
}

void channel_serverPrintBlink(Channel *self, const char *str){
	if(CHANNEL_SERVER_PRINT_CONDITION){
		ton_reset(&self->tmr);
		channel_printStrBlink(self, str);
		self->control = channel_SERVER;
	}
}

int CHANNEL_FUN_GET(enable)(Channel *self){return self->enable;}
int CHANNEL_FUN_GET(device_kind)(Channel *self){return self->device_kind;}
int CHANNEL_FUN_GET(display_kind)(Channel *self){return self->display.kind;}
int CHANNEL_FUN_GET(display_p1)(Channel *self){return self->display.p1;}
int CHANNEL_FUN_GET(display_p2)(Channel *self){return self->display.p2;}
int CHANNEL_FUN_GET(display_p3)(Channel *self){return self->display.p3;}
int CHANNEL_FUN_GET(display_text_alignment)(Channel *self){return self->display_text_alignment;}
int CHANNEL_FUN_GET(serial_id)(Channel *self){return self->serial_id;}
int CHANNEL_FUN_GET(mode)(Channel *self){return self->mode;}
int CHANNEL_FUN_GET(remote_id)(Channel *self){return self->remote_id;}
int CHANNEL_FUN_GET(acp_command)(Channel *self){return self->acp_command;}
unsigned long CHANNEL_FUN_GET(time)(Channel *self){return self->tmr.interval;}
int CHANNEL_FUN_GET(float_precision)(Channel *self){return self->float_precision;}

