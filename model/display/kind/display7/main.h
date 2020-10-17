#ifndef DISPLAY_DISPLAY7_H
#define DISPLAY_DISPLAY7_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../common.h"
#include "../../blink.h"
#include "../../scroll.h"
#include "../../../interface/iDisplay.h"
#include "../../interface/i7Segment.h"
#include "MAX7219.h"
#include "TM1637.h"

typedef struct display7_st Display7;
struct display7_st{
	i7Segment *device;
	size_t (*buildBufOfStr)(uint8_t *, size_t, const char *);
	void (*setSigns)(Display7 *, const char *);
	int alignment;
	int mode;
	uint8_t brightness;
	uint8_t *signs;
	size_t signs_count;
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen;
	iBlink im_blink;
	Blink blink;
	Scroll scroll;
	void (*control) (void *);
	iDisplay im_display;
};

extern Display7 *display7_new();

extern int display7_begin(Display7 *item, int device_kind, int din, int clk, int cs);

#endif
