#include "dsled.h"

#define FOREACH_PIN for(size_t i = 0; i<DSLED_PIN_COUNT; i++) {int pin = self->pin[i];
#define TURN_ON_LED(V)	if(self->last_high_pin != DSLED_UNKNOWN_PIN) digitalWrite(self->last_high_pin, LOW); \
						digitalWrite(self->pin[V], HIGH); \
						self->last_high_pin = self->pin[V];
#define STATE_IS(V) strcmp(V, str) == 0
#define TURN_OFF_ALL_LEDS FOREACH_PIN digitalWrite(pin, LOW);}	self->last_high_pin = DSLED_UNKNOWN_PIN;
#define RESET_ERROR if(self->control == dsled_ERROR) {blink_stopLow(&self->blink); self->im_blink.high = dsled_blinkHigh; self->im_blink.low = dsled_blinkLow;}

void dsled_IDLE(DSLed *self){
	;
}

void dsled_OFF(DSLed *self){
	;
}

void dsled_FAILURE(DSLed *self){
	;
}

void dsled_RUN(DSLed *self){
	;
}

void dsled_DONE(DSLed *self){
	CONTROL(&self->blink);
}

void dsled_ERROR(DSLed *self){
	CONTROL(&self->blink);
}

void dsled_PAUSE(DSLed *self){
	CONTROL(&self->blink);
}

void dsled_blinkHigh(void *vself);

void dsled_blinkLow(void *vself);
	
void dsled_blinkHighAll(void *vself);

void dsled_blinkLowAll(void *vself);
	
void dsled_printStr(void *vself, const char *str, int alignment){
	DSLed *self = (DSLed *) vself;
	if(STATE_IS("RUN")){
		if(self->control == dsled_RUN) return;
		RESET_ERROR
		blink_stopLow(&self->blink);
		TURN_ON_LED(DSLED_RUN_PIN)
		self->control = dsled_RUN;
	} else if(STATE_IS("OFF")){
		if(self->control == dsled_OFF) return;
		RESET_ERROR
		blink_stopLow(&self->blink);
		TURN_ON_LED(DSLED_OFF_PIN)
		self->control = dsled_OFF;
	} else if(STATE_IS("DONE")){
		if(self->control == dsled_DONE) return;
		RESET_ERROR
		blink_start(&self->blink);
		TURN_ON_LED(DSLED_RUN_PIN)
		self->control = dsled_DONE;
	} else if(STATE_IS("PAUSE")){
		if(self->control == dsled_PAUSE) return;
		RESET_ERROR
		blink_start(&self->blink);
		TURN_ON_LED(DSLED_OFF_PIN)
		self->control = dsled_PAUSE;
	}else if (STATE_IS("FAILURE")){
		if(self->control == dsled_FAILURE) return;
		RESET_ERROR
		blink_stopLow(&self->blink);
		TURN_ON_LED(DSLED_FAILURE_PIN)
		self->control = dsled_FAILURE;
	}
}

//indicate an error
void dsled_printBlinkStr(void *vself, const char *str, int alignment){
	DSLed *self = (DSLed *) vself;
	if(self->control == dsled_ERROR) return;
	TURN_OFF_ALL_LEDS
	blink_stopLow(&self->blink);
	self->im_blink.high = dsled_blinkHighAll;
	self->im_blink.low = dsled_blinkLowAll;
	blink_start(&self->blink);
	self->control = dsled_ERROR;
}

void dsled_control(void *vself){
	DSLed *self = (DSLed *) vself;
	self->control(self);
}

void dsled_free(void *vself){
	printdln("DISPLAY FREE");;
}

void dsled_clear(void *vself){
	DSLed *self = (DSLed *) vself;
	TURN_OFF_ALL_LEDS
}

void dsled_blinkHigh(void *vself){
	DSLed *self = (DSLed *) vself;
	digitalWrite(self->last_high_pin, HIGH);
}

void dsled_blinkLow(void *vself){
	DSLed *self = (DSLed *) vself;
	digitalWrite(self->last_high_pin, LOW);
}

void dsled_blinkHighAll(void *vself){
	DSLed *self = (DSLed *) vself;
	FOREACH_PIN
		digitalWrite(pin, HIGH);
	}
}

void dsled_blinkLowAll(void *vself){
	DSLed *self = (DSLed *) vself;
	FOREACH_PIN
		digitalWrite(pin, LOW);
	}
}

int dsled_buildInterfaces(DSLed *self){
	self->im_blink.self = self;
	self->im_blink.high = dsled_blinkHigh;
	self->im_blink.low = dsled_blinkLow;
	self->im_display.self = self;
	self->im_display.clear = dsled_clear;
	self->im_display.control = dsled_control;
	self->im_display.free = dsled_free;
	self->im_display.printStr = dsled_printStr;
	self->im_display.printBlinkStr = dsled_printBlinkStr;
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

int dsled_begin(DSLed *self, int run_pin, int off_pin, int failure_pin){
	dsled_buildInterfaces(self);
	self->pin[DSLED_RUN_PIN] = run_pin;
	self->pin[DSLED_OFF_PIN] = off_pin;
	self->pin[DSLED_FAILURE_PIN] = failure_pin;
	FOREACH_PIN
		pinMode(pin, OUTPUT);
	}
	TURN_OFF_ALL_LEDS
	blink_begin(&self->blink, &self->im_blink);
	self->im_blink.high = dsled_blinkHighAll;
	self->im_blink.low = dsled_blinkLowAll;
	blink_start(&self->blink);
	self->control = dsled_ERROR;
	return 1;
}

#undef FOREACH_PIN
#undef TURN_ON_LED
#undef STATE_IS
#undef TURN_OFF_ALL_LEDS
