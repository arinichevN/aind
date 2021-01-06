#include "serial.h"



static void appSerial_controlIdle(AppSerial *item){
	;
}

static void appSerial_freeIdle(AppSerial *serial){
	;
}

void appSerial_reset(AppSerial *self){
	self->id = SERIAL_ID_NONE;
	self->mode = SERIAL_MODE_IDLE;
	self->device = NULL;
	self->controller = NULL;
	self->control = appSerial_controlIdle;
	self->free = appSerial_freeIdle;
}

static void appSerial_beginDevice(AppSerialParam *item, HardwareSerial *serial){
	if(item->mode != SERIAL_MODE_IDLE){
		unsigned long rate = serial_getRate(item->rate);
		int config = serial_getArduinoConfig(item->config);
		serial->begin(rate, config);
		while(!(*serial)){};
	}
}

#ifdef SERIAL_SERVER
static void appSerial_controlServer(AppSerial *item){
	Acpls *controller = (Acpls *) item->controller;
	acpls_control(controller, item->device);
}

static void appSerial_freeServer(AppSerial *serial){
	Acpls *item = (Acpls *) serial->controller;
	acpls_free(item);
}

static int appSerial_beginServer(AppSerial *item){
	Acpls *controller = NULL;
	if(!acpls_begin(&controller, &server_nodes)) {
		printd("server begin failed where id = "); printdln(item->id);
		return 0;
	}
	item->controller = (void *) controller;
	item->control = appSerial_controlServer;
	item->free = appSerial_freeServer;
	return 1;
}
#endif

static void appSerial_controlClient(AppSerial *item){
	Acplcm *controller = (Acplcm *) item->controller;
	acplcm_control(controller, item->device);
}

static void appSerial_controlSpy(AppSerial *item){
	Acply *controller = (Acply *) item->controller;
	acply_control(controller, item->device);
}

static void appSerial_freeSpy(AppSerial *serial){
	Acply *item = (Acply *) serial->controller;
	acply_free(item);
}

static void appSerial_freeClient(AppSerial *serial){
	Acplcm *item = (Acplcm *) serial->controller;
	acplcm_free(item);
}

static int appSerial_beginClient(AppSerial *item){
	Acplcm *controller = NULL;
	if(!acplcm_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlClient;
	item->free = appSerial_freeClient;
	return 1;
}

static int appSerial_beginSpy(AppSerial *item){
	Acply *controller = NULL;
	if(!acply_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlSpy;
	item->free = appSerial_freeSpy;
	return 1;
}

int appSerial_beginMode(AppSerial *serial, AppSerialParam *param, HardwareSerial **serial_debug){
	printd("serial "); printd(serial->id);printd(": ");
	switch (param->mode){
#ifdef SERIAL_SERVER
		case SERIAL_MODE_SERVER:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginServer(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			printdln(": server");
			break;
#endif
		case SERIAL_MODE_CLIENT:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginClient(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			printdln(" client");
			break;
		case SERIAL_MODE_SPY:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginSpy(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			printdln(" spy");
			break;
		case SERIAL_MODE_DEBUG:
			if(*serial_debug == NULL){
				appSerial_beginDevice(param, serial->device);
				*serial_debug = serial->device;
				serial->mode = param->mode;
				printdln(" debug");
			}
			break;
		default:
			printdln(" unknown");
			break;
	}
	return ERROR_NO;
}


