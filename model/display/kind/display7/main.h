#ifndef DISPLAY_DISPLAY7_H
#define DISPLAY_DISPLAY7_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../common.h"
#include "../../blink.h"
#include "../../scroll.h"
#include "MAX7219.h"
#include "TM1637.h"

typedef struct display7_st DISPLAY7;
struct display7_st{
	void *device;
	void (*device_printSigns)(void *, const uint8_t *);
	void (*device_clear)(void *device);
	size_t (*buildBufOfStr)(uint8_t *, size_t, const char *);
	void (*setSigns)(DISPLAY7 *item, const char *str);
	int alignment;
	int mode;
	uint8_t brightness;
	uint8_t *signs;
	size_t signs_count;
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen;
	Blink blink;
	Scroll scroll;
	void (*control) (void *);
};

extern DISPLAY7 *display7_new();

extern void display7_free(void *self);

extern int display7_begin(DISPLAY7 *item, int device_kind, int din, int clk, int cs);

extern void display7_printStr(void *self, const char *str, int alignment);

extern void display7_printBlinkStr(void *self, const char *str, int alignment);

extern void display7_clear(void *self);

extern void display7_control(void *self);

#endif
