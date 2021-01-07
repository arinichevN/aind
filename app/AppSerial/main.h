#ifndef APP_SERIAL_H
#define APP_SERIAL_H

#include "../../lib/serial.h"
#include "../../lib/acp/loop/main.h"
#include "../../lib/acp/loop/server/main.h"
#include "param.h"


typedef struct app_serial_st AppSerial;
struct app_serial_st {
	int id;
	HardwareSerial *device;
	void *controller;
	int mode;
	void (*control) (AppSerial *);
	void (*free) (AppSerial *);
};

DEC_LIST(AppSerial)

extern void appSerial_reset(AppSerial *self);
extern int appSerial_beginKind(AppSerial *serial, AppSerialParam *param, HardwareSerial **serial_debug);
#define appSerial_free(item) item->free(item)

#endif
