#ifndef DISPLAY_MAIN_H
#define DISPLAY_MAIN_H

#include "../../lib/common.h"
#include "../interface/iDisplay.h"
#include "../DSerial/main.h"
#include "../DSLed/main.h"
#include "../Display7/main.h"
#include "common.h"

typedef struct {
	iDisplay *device;
	dk_t kind;
	int p1;
	int p2;
	int p3;
} Display;

extern int display_begin(Display *self, dk_t device_kind, int p1, int p2, int p3);

extern void display_free(Display *self);

#define DISPLAY_CONTROL(D) (D)->device->control((D)->device->self)

#define DISPLAY_CONTROLN(D) D.device->control(D.device->self)

#define DISPLAY_PRINT_STR(D, STR, ALIGN) (D)->device->printStr((D)->device->self, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STR(D, STR, ALIGN) (D)->device->printBlinkStr((D)->device->self, STR, ALIGN)

#define DISPLAY_CLEARN(D) (D).device->clear((D).device->self)

#define DISPLAY_PRINT_STRN(D, STR, ALIGN) (D).device->printStr((D).device->self, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STRN(D, STR, ALIGN) (D).device->printBlinkStr((D).device->self, STR, ALIGN)

#endif
