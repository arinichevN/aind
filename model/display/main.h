#ifndef DISPLAY_MAIN_H
#define DISPLAY_MAIN_H

#include "common.h"
#include "../../util/common.h"

#include "dserial.h"
#include "MAX7219.h"
#include "TM1637.h"

typedef struct {
	void *device;
	int kind;
	void (*printStr) (void *, const char *, int);
	void (*printBlinkStr) (void *, const char *, int);
	void (*control) (void *);
} Display;

extern int display_begin(Display *item, int device_kind, int p1, int p2, int p3);

#define display_control(D) (D)->control((D)->device)

#define display_controln(D) D.control(D.device)

#define display_printStr(D, STR, ALIGN) (D)->printStr((D)->device, STR, ALIGN)

#define display_printBlinkStr(D, STR, ALIGN) (D)->printBlinkStr((D)->device, STR, ALIGN)

#define display_printStrn(D, STR, ALIGN) (D).printStr((D).device, STR, ALIGN)

#define display_printBlinkStrn(D, STR, ALIGN) (D).printBlinkStr((D).device, STR, ALIGN)

#endif
