#include "dsled.h"

#define FOREACH_PIN for(size_t i = 0; i<DSLED_PIN_COUNT; i++) {int pin = item->pin[i];
#define TURN_ON_LED(V)	if(item->last_high_pin != DSLED_UNKNOWN_PIN) digitalWrite(item->last_high_pin, LOW); \
						digitalWrite(item->pin[V], HIGH); \
						item->last_high_pin = item->pin[V];
#define STATE_IS(V) strcmp(V, str) == 0
#define TURN_OFF_ALL_LEDS FOREACH_PIN digitalWrite(pin, LOW);}	item->last_high_pin = DSLED_UNKNOWN_PIN;

void dsled_printStr(void *self, const char *str, int alignment){
	DSLed *item = (DSLed *) self;
	if(STATE_IS("FAILURE")){
		TURN_ON_LED(DSLED_FAILURE_PIN)
	} else if(STATE_IS("RUN")){
		TURN_ON_LED(DSLED_RUN_PIN)
	} else if(STATE_IS("STOP")){
		TURN_ON_LED(DSLED_STOP_PIN)
	}
}

void dsled_printBlinkStr(void *self, const char *str, int alignment){
	DSLed *item = (DSLed *) self;
	TURN_OFF_ALL_LEDS
}

void dsled_control(void *self){
	;
}

void dsled_free(void *self){
	printdln("DISPLAY FREE");;
}

void dsled_clear(void *self){
	DSLed *item = (DSLed *) self;
	TURN_OFF_ALL_LEDS
}

int dsled_buildInterfaces(DSLed *item){
	item->im_display.self = item;
	item->im_display.clear = dsled_clear;
	item->im_display.control = dsled_control;
	item->im_display.free = dsled_free;
	item->im_display.printStr = dsled_printStr;
	item->im_display.printBlinkStr = dsled_printBlinkStr;
	return 1;
}

DSLed *dsled_new(){
	size_t sz = sizeof (DSLed);
	DSLed *out = (DSLed *) malloc(sz);
	if(out == NULL){ printdln("dsled_new: failed");}
	memset(out, 0, sz);
	printd("dsled_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int dsled_begin(DSLed *item, int run_pin, int stop_pin, int failure_pin){
	item->pin[DSLED_RUN_PIN] = run_pin;
	item->pin[DSLED_STOP_PIN] = stop_pin;
	item->pin[DSLED_FAILURE_PIN] = failure_pin;
	FOREACH_PIN
		pinMode(pin, OUTPUT);
	}
	TURN_OFF_ALL_LEDS
	dsled_buildInterfaces(item);
	return 1;
}

#undef FOREACH_PIN
#undef TURN_ON_LED
#undef STATE_IS
#undef TURN_OFF_ALL_LEDS
