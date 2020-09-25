#ifndef DISPLAY_MAX7219_H
#define DISPLAY_MAX7219_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../util/ton.h"
#include "common.h"
#include "blink.h"

#define MAX7219_SIGNS_COUNT			8

#define MAX7219_REG_FIRST_DIGIT		0x01
#define MAX7219_REG_DECODE_MODE		0x09
#define MAX7219_REG_INTENSITY		0x0a
#define MAX7219_REG_SCAN_LIMIT		0x0b
#define MAX7219_REG_SHUTDOWN		0x0c
#define MAX7219_REG_DISPLAY_TEST	0x0f


#define MAX7219_INTENSITY_MIN		0x00
#define MAX7219_INTENSITY_MAX		0x0f
#define MAX7219_DECODE_MODE			0x00
#define MAX7219_SCAN_LIMIT			7
#define MAX7219_SHUTDOWN_STOP		1
#define MAX7219_DISPLAY_TEST_STOP	0
#define MAX7219_BRIGHTNESS 			MAX7219_INTENSITY_MAX

typedef struct max7219_st{
	int din;
	int clk;
	int cs;
	int alignment;
	uint8_t brightness;
	uint8_t signs[MAX7219_SIGNS_COUNT];
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen;
	size_t i;
	Ton tmr;
	Blink blink;
	void (*control) (void *);
} MAX7219;

extern MAX7219 *max7219_new();

extern void max7219_begin(MAX7219 *item, int din, int clk, int cs);

extern void max7219_printStr(void *device, const char *str, int alignment);

extern void max7219_printBlinkStr(void *device, const char *str, int alignment);

extern void max7219_clear(MAX7219 *item);

extern void max7219_setBrightness (MAX7219 *item, uint8_t brightness);

extern void max7219_control(void *device);

#endif
