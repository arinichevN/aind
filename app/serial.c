#include "serial.h"

static void appSerial_controlIdle(AppSerial *item){
	;
}

static void appSerial_freeIdle(AppSerial *serial){
	;
}

void appSerials_reset(AppSerial serials[]){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		serial->id = SERIAL_IDN;
		serial->kind = APP_SERIAL_KIND_IDLE;
		serial->device = NULL;
		serial->controller = NULL;
		serial->control = appSerial_controlIdle;
		serial->free = appSerial_freeIdle;
	}
}

void appSerials_init(AppSerial serials[]){
	appSerials_reset(serials);
	size_t i = 0;
#ifdef USE_SERIAL0
	serials[i].device = &Serial;
	serials[i].id = SERIAL_ID0; i++;
#endif
#ifdef USE_SERIAL1
	serials[i].device = &Serial1;
	serials[i].id = SERIAL_ID1; i++;
#endif
#ifdef USE_SERIAL2
	serials[i].device = &Serial2;
	serials[i].id = SERIAL_ID2; i++;
#endif
#ifdef USE_SERIAL3
	serials[i].device = &Serial3;
	serials[i].id = SERIAL_ID3; i++;
#endif
}

static void appSerial_beginDevice(AppSerialConfig *item, HardwareSerial *serial){
	if(item->kind != APP_SERIAL_KIND_IDLE){
		unsigned long rate = serial_getRate(item->rate);
		serial->begin(rate, item->config);
		while(!(*serial)){};
	}
}

static void appSerial_controlClient(AppSerial *item){
	ACPLCM *controller = (ACPLCM *) item->controller;
	acplcm_control(controller, item->device);
}

static void appSerial_controlSpy(AppSerial *item){
	ACPLY *controller = (ACPLY *) item->controller;
	acply_control(controller, item->device);
}

static void appSerial_freeSpy(AppSerial *serial){
	ACPLY *item = (ACPLY *) serial->controller;
	acply_free(item);
}

static void appSerial_freeClient(AppSerial *serial){
	ACPLCM *item = (ACPLCM *) serial->controller;
	acplcm_free(item);
}

static int appSerial_beginClient(AppSerial *item){
	ACPLCM *controller = NULL;
	if(!acplcm_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlClient;
	item->free = appSerial_freeClient;
	return 1;
}

static int appSerial_beginSpy(AppSerial *item){
	ACPLY *controller = NULL;
	if(!acply_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlSpy;
	item->free = appSerial_freeSpy;
	return 1;
}

int appSerial_beginKind(AppSerial *serial, AppSerialConfig *config, HardwareSerial **serial_debug){
	printd("serial "); printd(serial->id);printd(": ");
	switch (config->kind){
		case APP_SERIAL_KIND_CLIENT:
			appSerial_beginDevice(config, serial->device);
			if(!appSerial_beginClient(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->kind = config->kind;
			printdln(" client");
			break;
		case APP_SERIAL_KIND_SPY:
			appSerial_beginDevice(config, serial->device);
			if(!appSerial_beginSpy(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->kind = config->kind;
			printdln(" spy");
			break;
		case APP_SERIAL_KIND_DEBUG:
			if(*serial_debug == NULL){
				appSerial_beginDevice(config, serial->device);
				*serial_debug = serial->device;
				serial->kind = config->kind;
				printdln(" debug");
			}
			break;
		default:
			printdln(" unknown");
			break;
	}
	return ERROR_NO;
}

void appSerials_control(AppSerial serials[] ){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		CONTROL(serial);
	}
}

AppSerial *appSerials_getClientSerialById(AppSerial serials[], int id){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		if(id == serial->id && serial->kind == APP_SERIAL_KIND_CLIENT){
			return serial;
		}
	}
	return NULL;
}
