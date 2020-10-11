#include "spy.h"

#define ACPLY_CLIENT_LIST client_list
#define ACPLY_DEF_CLIENT_LIST ACPLYClientList *ACPLY_CLIENT_LIST = &item->client_list;
#define ACPLY_FOREACH_CLIENT  for(size_t i=0; i<ACPLY_CLIENT_LIST->length; i++){ACPLYClient *client = &ACPLY_CLIENT_LIST->items[i];

int acply_initClients(ACPLY *item, size_t count){
	ACPLY_DEF_CLIENT_LIST
	ACPLY_CLIENT_LIST->items = NULL;
	ACPLY_CLIENT_LIST->length = 0;
	ACPLY_CLIENT_LIST->items = (ACPLYClient *) malloc(sizeof (*ACPLY_CLIENT_LIST->items) * count);
	if(ACPLY_CLIENT_LIST->items == NULL){
		printdln("acply_initClients failed to allocate memory");
		return 0;
	}
	ACPLY_CLIENT_LIST->length = count;
	printd("acply clients array length = "); printdln(ACPLY_CLIENT_LIST->length);
	ACPLY_FOREACH_CLIENT
		client->self = NULL;
	}
	return 1;
}

int acply_addClient(ACPLY *item, void *vclient, int (*onRequestFunction) (void *, char *, int, int), void (*onResponseFunction)(void *, char *, int, int)){
	ACPLY_DEF_CLIENT_LIST
	ACPLY_FOREACH_CLIENT
		if(client->self == NULL){
			client->self = vclient;
			client->onRequestFunction = onRequestFunction;
			client->onResponseFunction = onResponseFunction;
			return 1;
		}
	}
	return 0;
}

int acply_delClient(ACPLY *item, void *client){
	ACPLY_DEF_CLIENT_LIST
	ACPLY_FOREACH_CLIENT
		if(client->self == client){
			client->self = NULL;
			return 1;
		}
	}
	return 0;
}

void acply_free(ACPLY *item) {
	if(item == NULL) return;
	acpl_free(item->acpl);
	free(item->client_list.items);
	item->client_list.items = NULL;
	free(item);
}

void acply_init(ACPLY *item){
	item->client_list.items = NULL;
	item->client_list.length = 0;
	item->last_id = -63;
}

void acply_READ_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) ;

ACPLY *acply_new(){
	size_t sz = sizeof (ACPLY);
	ACPLY *out = (ACPLY *) malloc(sz);
	if(out == NULL){ printdln("acply_new: failed");}
	printd("acply_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acply_begin(ACPLY **item){
	ACPLY *titem = acply_new();
	if(titem == NULL) return 0;
	titem->acpl = NULL;
	if(!acpl_begin(&titem->acpl)) return 0;
	acply_init(titem);
	titem->control = acply_READ_REQUEST;
	*item = titem;
	return 1;
}

void acply_reset(ACPLY *item){
	acpl_reset(item->acpl);
	item->control = acply_READ_REQUEST;
}

int acply_setParam(ACPLY *item, size_t client_count) {
	if(!acply_initClients(item, client_count)){
		printdln("acply_Init: failed");
		return 0;
	}
	return 1;
}

void acply_READ_REQUEST(ACPLY *item, HardwareSerial *serial) {
	int r = acpl_readRequest(item->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: item->control = acply_CONSIDER_REQUEST; return;
	}
	printd("acply_READ_REQUEST failed: "); printdln(acp_getStateStr(r));
	acply_reset(item); 
}

void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial) {
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		char sign = acpl->buf[ACP_IND_SIGN];
		if(sign == ACP_SIGN_REQUEST_GET || sign == ACP_SIGN_REQUEST_SET || sign == ACP_SIGN_REQUEST_SET_BROADCAST || sign == ACP_SIGN_REQUEST_GET_BROADCAST){
			int cmd;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_CMD, &cmd)){
				//printdln("request: failed to read cmd");
				acply_reset(item); return;
			}
			int id;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_ID, &id)){
				//printdln("request: failed to read channel id");
				acply_reset(item); return;
			}
			item->last_id = id;
			int response_required = 0;
			ACPLY_DEF_CLIENT_LIST
			ACPLY_FOREACH_CLIENT
				if(client->self != NULL){
					response_required = response_required || client->onRequestFunction(client->self, acpl->buf, id, cmd);
				}
			}
			if(!response_required){
				//printdln("acply: no channels need response to this request");
				acply_reset(item); return;
			}
		}else{
			//printdln("acply: not request sign");
			acply_reset(item); return;
		}
	}else{
		printdln("acply: bad request crc");
		acply_reset(item); return;
	}
	acply_reset(item);
	item->control = acply_READ_RESPONSE;
}

void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int r = acpl_readResponse(item->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: item->control = acply_CONSIDER_RESPONSE; return;
	}
	printd("acply_READ_RESPONSE failed: "); printdln(acp_getStateStr(r));
	acply_reset(item); 
}

void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int id;
	int success = 0;
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_IND_SIGN] == ACP_SIGN_RESPONSE){
			if(acp_packGetCellI(acpl->buf, ACP_RESPONSE_IND_ID, &id)){
				if(item->last_id == id){
					success = 1;
				}else{
				printdln("acply: bad response id");
			}
			}else{
				printdln("acply: failed to get response id");
			}
		}else{
			printdln("acply: not response sign");
		}
	}else{
		printdln("acply: bad response crc");
	}
	ACPLY_DEF_CLIENT_LIST
	ACPLY_FOREACH_CLIENT
		if(client->self != NULL){
			client->onResponseFunction(client->self, acpl->buf, id, success);
		}
	}
	acply_reset(item);
}
