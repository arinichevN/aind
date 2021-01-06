#ifndef DISPLAY_TM1637_H
#define DISPLAY_TM1637_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../../interface/i7Segment.h"
#include "../../interface/iScrollSlave.h"

#define TM1637_SIGNS_COUNT		4

#define TM1637_MOD_POS			0

#define TM1637_I2C_COMM1		0x40
#define TM1637_I2C_COMM2		0xC0
#define TM1637_I2C_COMM3		0x80

#define TM1637_BRIGHTNESS		7

#define TM1637_DELAY			delayMicroseconds(100);

typedef struct tm1637_st{
	int dio;
	int clk;
	uint8_t brightness;
	i7Segment im_7segment;
	iScrollSlave im_scroll;
} TM1637;

extern TM1637 *tm1637_new();

extern void tm1637_begin(TM1637 *self, int dio, int clk);

#endif
