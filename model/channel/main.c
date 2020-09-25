#include "main.h"

Channel channel_buf[CHANNEL_COUNT];

static void channel_spy(Channel *item);

static void channel_clientWait(Channel *item);

static void channel_clientFTSGet(Channel *item);

static void channel_clientStrGet(Channel *item);

static void channel_INIT(Channel *item);

static void channel_OFF(Channel *item);

static void channel_FAILURE(Channel *item);

static void channel_printFloatFull(Channel *item, double v);

static void channel_printFloatInt(Channel *item, double v);

static void channel_parseNshowSpyNothing(Channel *item, char *buf);

static void channel_parseNshowSpyFTS(Channel *item, char *buf);

static void channel_parseNshowSpyStr(Channel *item, char *buf);

static int channel_parseNShowSpyRequestIdle(Channel *item, char *buf);

static int channel_parseNShowSpyRequestSkip(Channel *item, char *buf);

static int channel_parseNShowSpyRequestFloat(Channel *item, char *buf);

static int channelParam_check(const ChannelParam *item){
	if(!common_checkBlockStatus(item->enable)){
		printd("channelParam_check(): bad enable where id = "); printdln(item->id);
		return ERROR_PARAM;
	}
	switch(item->mode){
		case CHANNEL_MODE_SPY:
		case CHANNEL_MODE_CLIENT:
			break;
		default:
			printd("channelParam_check(): bad mode where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
	switch(item->display_kind){
		case DEVICE_KIND_MAX7219:
		case DEVICE_KIND_TM1637:
		case DEVICE_KIND_DSERIAL:
			break;
		default:
			printd("channelParam_check(): bad display_kind where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
	switch(item->display_text_alignment){
		case DISPLAY_ALIGNMENT_LEFT:
		case DISPLAY_ALIGNMENT_RIGHT:
			break;
		default:
			printd("channelParam_check(): bad display_text_alignment where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
	switch(item->acp_command){
		case CMD_GETR_CHANNEL_FTS:
		case CMD_GETR_CHANNEL_STATE:
		case CMD_GETR_CHANNEL_ERROR:
		case CMD_SET_CHANNEL_GOAL:
			break;
		default:
			printd("channelParam_check(): bad acp_command where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
	switch(item->acp_command){
		case CMD_SET_CHANNEL_GOAL:
		if(item->mode != CHANNEL_MODE_SPY){
			printd("channelParam_check(): spy mode expected for set acp command where id = "); printdln(item->id);
			return ERROR_PARAM;
		}
	}
	switch(item->serial_id){
		case SERIAL_IDN:
		case SERIAL_ID0:
		case SERIAL_ID1:
		case SERIAL_ID2:
		case SERIAL_ID3:
			break;
		default:
			printd("channelParam_check(): bad serial_id where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
    return ERROR_NO;
}

const char *channel_getStateStr(Channel *item){
	if(item->control == channel_clientFTSGet || item->control == channel_clientStrGet || item->control == channel_clientWait || item->control == channel_spy){ 
		return "RUN";
	} else if(item->control == channel_OFF) {
		return "OFF";
	} else if(item->control == channel_FAILURE) {
		return "FAILURE";
	} else if(item->control == channel_INIT) {
		return "INIT";
	}
	return "?";
}

const char *channel_getErrorStr(Channel *item){
	return getErrorStr(item->error_id);
}

static int channel_setDefaults(Channel *item, size_t ind){
	const ChannelParam *param = &CHANNEL_DEFAULT_PARAMS[ind];
	int r = channelParam_check(param);
	if(r != ERROR_NO){
		return r;
	}
	item->remote_id = param->remote_id;
	if(!display_begin(&item->display, param->display_kind, param->display_p1, param->display_p2, param->display_p3)){
		return ERROR_SOME;
	}
	switch(item->display.kind){
		case DEVICE_KIND_MAX7219:
			item->printFloat = channel_printFloatFull;
			break;
		case DEVICE_KIND_TM1637:
			item->printFloat = channel_printFloatInt;
			break;
		case DEVICE_KIND_DSERIAL:
			item->printFloat = channel_printFloatFull;
			break;
		default:
			item->printFloat = NULL;
			printd("channel_setDefaults(): bad display_kind where id = "); printdln(item->id);
			return ERROR_PARAM;
	}
	item->display_text_alignment = param->display_text_alignment;
	item->mode = param->mode;
	item->acp_command = param->acp_command;
	item->parseNshowSpyData = channel_parseNshowSpyNothing;
	item->parseNShowSpyRequestData = channel_parseNShowSpyRequestIdle;
	switch(item->acp_command){
		case CMD_GETR_CHANNEL_FTS:
			item->parseNshowSpyData = channel_parseNshowSpyFTS;
			item->parseNShowSpyRequestData = channel_parseNShowSpyRequestSkip;
			break;
		case  CMD_GETR_CHANNEL_STATE:
		case CMD_GETR_CHANNEL_ERROR:
			item->parseNshowSpyData = channel_parseNshowSpyStr;
			item->parseNShowSpyRequestData = channel_parseNShowSpyRequestSkip;
			break;
		case CMD_SET_CHANNEL_GOAL:
			item->parseNShowSpyRequestData = channel_parseNShowSpyRequestFloat;
			break;
	}
	ton_setInterval(&item->tmr, param->time);
	item->serial_id = param->serial_id;
	item->id = param->id;
	item->enable = param->enable;
	return ERROR_NO;
}

static int channel_setParam(Channel *item, size_t ind){
	int r = channel_setDefaults(item, ind);
	if(r != ERROR_NO){
		return r;
	}
	item->ind = ind;
	item->device_kind = DEVICE_KIND_INDICATOR;
	return ERROR_NO;
}

void channel_begin(Channel *item, size_t ind){
	printd("beginning channel ");printd(ind); printdln(":");
	item->error_id = channel_setParam(item, ind);
	item->control = channel_INIT;
	printd("\tid: ");printdln(item->id);
	printd("\n");
}

void channels_buildFromArray(ChannelLList *channels, Channel arr[]){
	if(CHANNEL_COUNT <= 0) return;
	channels->length = CHANNEL_COUNT;
	channels->top = &arr[0];
	channels->last = &arr[CHANNEL_COUNT - 1];
	for(size_t i = 0;i<CHANNEL_COUNT;i++){
		arr[i].next = NULL;
	}
	for(size_t i = 0; i<CHANNEL_COUNT-1; i++){
		arr[i].next = &arr[i+1];
	}
}

void channels_begin(ChannelLList *channels){
	extern Channel channel_buf[CHANNEL_COUNT];
	channels_buildFromArray(channels, channel_buf);
	size_t i = 0;
	FOREACH_CHANNEL(channels){
		channel_begin(channel, i); i++;
	}
}

void channel_printStr(Channel *item, const char *str){
	display_printStrn(item->display, str, item->display_text_alignment);
}

void channel_printError(Channel *item, const char *str){
	display_printBlinkStrn(item->display, str, item->display_text_alignment);
}

void channel_printFloat(Channel *item, double v){
	item->printFloat(item, v);
}

static void channel_printFloatFull(Channel *item, double v){
	char str[CHANNEL_DISPLAY_ROWS];
	dtostrf(v, 0, CHANNEL_FLOAT_PRECISION, str);
	display_printStrn(item->display, str, item->display_text_alignment);
}

static void channel_printFloatInt(Channel *item, double v){
	char str[CHANNEL_DISPLAY_ROWS];
	dtostrf(v, 0, 0, str);
	display_printStrn(item->display, str, item->display_text_alignment);
}

static void channel_parseNshowSpyNothing(Channel *item, char *buf){
	channel_printError(item, CHANNEL_MSG_SPY_FAILED);
	printd("channel "); printd(item->id); printdln(": no parseNshow function");
}

static void channel_parseNshowSpyFTS(Channel *item, char *buf){
	double v;
	if(!acp_packGetCellF (buf, ACP_RESPONSE_IND_PARAM1, &v)){
		channel_printError(item, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	int state;
	if(!acp_packGetCellI (buf, ACP_RESPONSE_IND_PARAM4, &state)){
		channel_printError(item, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	if(!state){
		channel_printError(item, CHANNEL_MSG_BAD_STATE);
		return;
	}
	channel_printFloat(item, v);
	printd(item->id); printd(" spy fts: "); printdln(v);
}

static void channel_parseNshowSpyStr(Channel *item, char *buf){
	char s[CHANNEL_DISPLAY_ROWS];
	memset(s, 0, sizeof s);
	if(!acp_packGetCellS (buf, ACP_RESPONSE_IND_PARAM1, s, CHANNEL_DISPLAY_ROWS)){
		channel_printError(item, CHANNEL_MSG_PARSE_FAILED);
		return;
	}
	channel_printStr(item, s);
	printd(item->id); printd(" spy str: "); printdln(s);
}

static int channel_parseNShowSpyRequestIdle(Channel *item, char *buf){
	int need_spy_response = 0;
	return need_spy_response;
}

static int channel_parseNShowSpyRequestSkip(Channel *item, char *buf){
	int need_spy_response = 1;
	return need_spy_response;
}

static int channel_parseNShowSpyRequestFloat(Channel *item, char *buf){
	double v;
	if(acp_packGetCellF(buf, ACP_REQUEST_IND_PARAM1, &v)){
		channel_printFloat(item, v);
		printd(item->id); printd(" spy float: "); printdln(v);
	}else{
		channel_printError(item, CHANNEL_MSG_PARSE_FAILED);
	}
	ton_reset(&item->tmr);
	item->control = channel_spy;
	int need_spy_response = 0;
	return need_spy_response;
}

int channel_onSpyRequest(void *vitem, char *buf, int id, int command){
	Channel *item = (Channel *) vitem;
	item->need_spy_response = 0;
	if(!item->enable){
		return item->need_spy_response;
	}
	if(id != item->remote_id){
		printd("channel "); printd(item->id); printd(": bad id = ");  printd(id); printdln(" in request"); 
		return item->need_spy_response;
	}
	if(command != item->acp_command){
		printd("channel "); printd(item->id); printd(": bad command = ");  printd(command); printdln(" in request"); 
		return item->need_spy_response;
	}
	item->need_spy_response = item->parseNShowSpyRequestData(item, buf);
	printd("channel "); printd(item->id); printdln(" wanted request detected");
	return item->need_spy_response;
}

void channel_onSpyResponse(void *vitem, char *buf, int id, int success){
	Channel *item = (Channel *) vitem;
	printd("channel "); printd(item->id); printdln(" wanted response detected");
	printd(buf);
	if(!success){
		item->need_spy_response = 0;
		channel_printError(item, CHANNEL_MSG_SPY_FAILED);
		printdln("printerror");
		return;
	}
	if(!item->need_spy_response) {
		printd("channel "); printd(item->id); printdln(" unwanted response"); 
		return;
	}
	if(id != item->remote_id){
		printd("channel "); printd(item->id); printd(": bad id = ");  printd(id); printdln(" in response"); 
		return;
	}
	item->parseNshowSpyData(item, buf);
	ton_reset(&item->tmr);
	item->control = channel_spy;
}


void channel_setControlFunction(Channel *item){
	item->control = NULL;
	switch(item->mode){
		case CHANNEL_MODE_CLIENT:
			switch(item->acp_command){
				case CMD_GETR_CHANNEL_FTS:		item->control = channel_clientFTSGet; item->control_next = channel_clientWait; return;
				case CMD_GETR_CHANNEL_STATE:	item->control = channel_clientStrGet; item->control_next = channel_clientWait; return;
				case CMD_GETR_CHANNEL_ERROR:	item->control = channel_clientStrGet; item->control_next = channel_clientWait; return;
			}
			break;
		case CHANNEL_MODE_SPY:
			switch(item->acp_command){
				case CMD_GETR_CHANNEL_FTS:
				case CMD_GETR_CHANNEL_STATE:
				case CMD_GETR_CHANNEL_ERROR:
				case CMD_SET_CHANNEL_GOAL:
					item->control = channel_spy;  item->control_next = channel_FAILURE; return;
			}
			break;
	}
}

size_t channels_countSpys(ChannelLList *channels){
	size_t c = 0;
	FOREACH_CHANNEL(channels){
		if(channel->mode == CHANNEL_MODE_SPY){
			c++;
		}
	}
	return c;
}

size_t channels_countSpysForSerial(ChannelLList *channels, int serial_id){
	size_t c = 0;
	FOREACH_CHANNEL(channels){
		if(channel->mode == CHANNEL_MODE_SPY && channel->serial_id == serial_id){
			c++;
		}
	}
	return c;
}

size_t channels_assignToSpySerial(ChannelLList *channels,  AppSerial *serial){
	size_t assigned_channels_count = 0;
	FOREACH_CHANNEL(channels){
		if(channel->mode == CHANNEL_MODE_SPY && channel->serial_id == serial->id){
			acply_addClient((ACPLY *) serial->controller, (void *) channel);
			assigned_channels_count++;
		}
	}
	return assigned_channels_count;
}

int channels_coopSerials(ChannelLList *channels, AppSerial serials[]){
	size_t total_spy_channels = channels_countSpys(channels);
	size_t assigned_spy_channels = 0;
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		if(serial->kind == APP_SERIAL_KIND_SPY){
			size_t sc = channels_countSpysForSerial(channels, serial->id);
			printd("serial"); printd(serial->id); printd(" number of spy channels: "); printdln(sc);
			if(sc > 0){
				if(!acply_setParam((ACPLY *) serial->controller, sc, &channel_onSpyRequest, &channel_onSpyResponse)){
					return 0;
				}
			}
		}
		assigned_spy_channels += channels_assignToSpySerial(channels, serial);
	}
	if(assigned_spy_channels != total_spy_channels){
		printd("channels_coopSerials: we have "); printd(total_spy_channels); printd(" spy channels"); printd(" but assigned "); printdln(assigned_spy_channels);
		return 0;
	}
	FOREACH_CHANNEL(channels){
		if(channel->mode == CHANNEL_MODE_CLIENT){
			channel->acplcm = NULL;
			AppSerial *serial = appSerials_getClientSerialById(serials, channel->serial_id);
			if(serial != NULL){
				channel->acplcm = (ACPLCM *) serial->controller;
				printd("serial"); printd(serial->id); printd(" client assigned to channel.id = "); printdln(channel->id);
			}
			if(channel->acplcm == NULL){
				channel->error_id = ERROR_NO_SERIAL;
				printd("(channels_coopSerials: bad serial: "); printd(channel->serial_id); printdln(" (expected client)) "); 
			}
		}
	}
	return 1;
	
}

static void channel_spyFailed(Channel *item){
	display_controln(item->display);
}

static void channel_spy(Channel *item){
	if(ton(&item->tmr)){
		channel_printError(item, CHANNEL_MSG_TIMEOUT);
		item->control = channel_spyFailed;
		//printd(item->id); printdln(" spy: timeout"); 
	}
	display_controln(item->display);
}

static void channel_clientWait(Channel *item){
	if(ton(&item->tmr)){
		item->control = item->control_next;
	}
	display_controln(item->display);
}

static void channel_clientFTSGet(Channel *item){
	FTS out;
	int r = acplcm_getFTS(item->acplcm, item, item->acp_command, item->remote_id, &out);
	switch(r){
		case ACP_WAIT:
		case ACP_BUSY:
			break;
		case ACP_DONE:
			if(!out.state){
				channel_printError(item, CHANNEL_MSG_BAD_STATE);
				printd(item->id); printdln(" fts: bad state");
			}else{
				channel_printFloat(item, out.value);
				printd(item->id); printd(" fts: "); printdln(out.value);
			}
			printdln(" ");
			ton_reset(&item->tmr);
			item->control = channel_clientWait;
			break;
		default://error
			channel_printError(item, CHANNEL_MSG_CLIENT_FAILED);
			ton_reset(&item->tmr);
			item->control = channel_clientWait;
			printd(item->id); printdln(" fts: failed: "); printd(" str: failed: "); printd(acp_getStateStr(r)); printdln(" ");
			break;
	}
	display_controln(item->display);
}

static void channel_clientStrGet(Channel *item){
	char out[CHANNEL_DISPLAY_ROWS];
	int r = acplcm_getIS(item->acplcm, item, item->acp_command, item->remote_id, out, CHANNEL_DISPLAY_ROWS);
	switch(r){
		case ACP_WAIT:
		case ACP_BUSY:
			break;
		case ACP_DONE:
			channel_printStr(item, out);
			ton_reset(&item->tmr);
			item->control = channel_clientWait;
			printd(item->id); printd(" str: "); printdln(out);printdln(" ");
			break;
		default://error
			channel_printError(item, CHANNEL_MSG_CLIENT_FAILED);
			ton_reset(&item->tmr);
			item->control = channel_clientWait;
			printd(item->id); printd(" str: failed: "); printd(acp_getStateStr(r)); printdln(" ");
			break;
	}
	display_controln(item->display);
}

static void channel_INIT(Channel *item){
	if(item->error_id != ERROR_NO){
		printd("channel_INIT "); printd(item->id); printdln(" error");
        item->control = channel_FAILURE;
        return;
    }
    channel_setControlFunction(item);
    if(item->control == NULL){
		printd("channel_INIT "); printd(item->id); printdln(" control function error");
        item->control = channel_FAILURE;
        return;
    }
	if(item->enable != YES){
		item->control = channel_OFF;
		return;
	}
	ton_expire(&item->tmr);
	channel_printStr(item, CHANNEL_MSG_INIT);
}

static void channel_OFF(Channel *item){
	;
}

static void channel_FAILURE(Channel *item){
	;
}
