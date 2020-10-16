#ifndef DISPLAY_MAIN_H
#define DISPLAY_MAIN_H

#include "common.h"
#include "../../util/common.h"

#include "kind/dserial.h"
#include "kind/MAX7219.h"
#include "kind/TM1637.h"

typedef struct {
	void *device;
	int kind;
	int p1;
	int p2;
	int p3;
	void (*clear) (void *);
	void (*printStr) (void *, const char *, int);
	void (*printBlinkStr) (void *, const char *, int);
	void (*control) (void *);
} Display;

extern int display_begin(Display *item, int device_kind, int p1, int p2, int p3);

extern void display_free(Display *item);

#define DISPLAY_CONTROL(D) (D)->control((D)->device)

#define DISPLAY_CONTROLN(D) D.control(D.device)

#define DISPLAY_PRINT_STR(D, STR, ALIGN) (D)->printStr((D)->device, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STR(D, STR, ALIGN) (D)->printBlinkStr((D)->device, STR, ALIGN)

#define DISPLAY_CLEARN(D) (D).clear((D).device)

#define DISPLAY_PRINT_STRN(D, STR, ALIGN) (D).printStr((D).device, STR, ALIGN)

#define DISPLAY_PRINT_BLINK_STRN(D, STR, ALIGN) (D).printBlinkStr((D).device, STR, ALIGN)

#endif
