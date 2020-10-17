#ifndef DISPLAY_MAIN_H
#define DISPLAY_MAIN_H

#include "common.h"
#include "../../util/common.h"

#include "../interface/iDisplay.h"
#include "kind/dserial.h"
#include "kind/display7/main.h"

typedef struct {
	iDisplay *device;
	int kind;
	int p1;
	int p2;
	int p3;
	//void *device;
	//void (*free) (void *);
	//void (*clear) (void *);
	//void (*printStr) (void *, const char *, int);
	//void (*printBlinkStr) (void *, const char *, int);
	//void (*control) (void *);
} Display;

extern int display_begin(Display *item, int device_kind, int p1, int p2, int p3);

extern void display_free(Display *item);

#define DISPLAY_CONTROL(D) (D)->device->control((D)->device->self)

#define DISPLAY_CONTROLN(D) D.device->control(D.device->self)

#define DISPLAY_PRINT_STR(D, STR, ALIGN) (D)->device->printStr((D)->device->self, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STR(D, STR, ALIGN) (D)->device->printBlinkStr((D)->device->self, STR, ALIGN)

#define DISPLAY_CLEARN(D) (D).device->clear((D).device->self)

#define DISPLAY_PRINT_STRN(D, STR, ALIGN) (D).device->printStr((D).device->self, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STRN(D, STR, ALIGN) (D).device->printBlinkStr((D).device->self, STR, ALIGN)

#endif
