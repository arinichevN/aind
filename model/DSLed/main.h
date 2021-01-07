#ifndef DISPLAY_DSLED_H
#define DISPLAY_DSLED_H

/*
 * this object is intended to display state of remote
 * channel using some LEDs
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../Display/common.h"
#include "../Blink/main.h"
#include "../interface/iBlink.h"
#include "../interface/iDisplay.h"

#define DSLED_UNKNOWN_PIN	0

#define DSLED_PIN_COUNT		3
#define DSLED_RUN_PIN		0 //array index
#define DSLED_OFF_PIN		1 //array index
#define DSLED_FAILURE_PIN	2 //array index

typedef struct dsled_st DSLed;
struct dsled_st {
	int pin[DSLED_PIN_COUNT];
	int last_high_pin;
	iBlink im_blink;
	Blink blink;
	iDisplay im_display;
	void (*control)(DSLed *);
};

extern int dsled_begin(DSLed *self, int run_pin, int off_pin, int failure_pin);

#endif
