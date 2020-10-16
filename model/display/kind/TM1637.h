#ifndef DISPLAY_TM1637_H
#define DISPLAY_TM1637_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../../util/ton.h"
#include "../common.h"
#include "../blink.h"
#include "../scroll.h"

#define TM1637_MOD_POS			0
#define TM1637_SIGNS_COUNT		4

#define TM1637_I2C_COMM1		0x40
#define TM1637_I2C_COMM2		0xC0
#define TM1637_I2C_COMM3		0x80

#define TM1637_BRIGHTNESS		7

#define TM1637_DELAY			delayMicroseconds(100);

typedef struct tm1637_st{
	int dio;
	int clk;
	int alignment;
	int mode;
	uint8_t brightness;
	uint8_t signs[TM1637_SIGNS_COUNT];
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen;
	//size_t i;
	//Ton tmr;
	Blink blink;
	Scroll scroll;
	void (*control) (void *);
} TM1637;

extern TM1637 *tm1637_new();

extern void tm1637_begin(TM1637 *item, int dio, int clk);

extern void tm1637_setBrightness(TM1637 *item, uint8_t brightness);

extern void tm1637_printStr(void *device, const char *str, int alignment);

extern void tm1637_printBlinkStr(void *device, const char *str, int alignment);

extern void tm1637_clear(void *device);

extern void tm1637_control(void *device);

#endif
