#ifndef DISPLAY_DISPLAY7_H
#define DISPLAY_DISPLAY7_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../lib/common.h"
#include "../Display/common.h"
#include "../Blink/main.h"
#include "../Scroll/main.h"
#include "../MAX7219/main.h"
#include "../TM1637/main.h"
#include "../interface/iDisplay.h"
#include "../interface/i7Segment.h"
#include "../interface/iScrollData.h"

typedef struct display7_st Display7;
struct display7_st{
	i7Segment *device;
	size_t (*buildBufOfStr)(uint8_t *, size_t, const char *);
	void (*setSigns)(Display7 *, const char *);
	talign_t alignment;
	int mode;
	//uint8_t brightness;
	uint8_t *signs;
	size_t signs_count;
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen;
	iBlink im_blink;
	Blink blink;
	iScrollData im_scroll_data;
	Scroll scroll;
	void (*control) (void *);
	iDisplay im_display;
};

extern Display7 *display7_new();

extern int display7_begin(Display7 *self, int device_kind, int din, int clk, int cs);

#endif
