#include "blink.h"

static void blink_IDLE(Blink *self);
static void blink_LOW(Blink *self);
static void blink_HIGH(Blink *self);

static void blink_IDLE(Blink *self){
	;
}

static void blink_LOW(Blink *self){
	if(tonr(&self->tmr)){
		self->slave->low(self->slave->self);
		self->control = blink_HIGH;
	}
}

static void blink_HIGH(Blink *self){
	if(tonr(&self->tmr)){
		self->slave->high(self->slave->self);
		self->control = blink_LOW;
	}
}

void blink_start(Blink *self){
	ton_expire(&self->tmr);
	self->control = blink_HIGH;	
}

void blink_stopLow(Blink *self){
	self->control = blink_IDLE;
	self->slave->low(self->slave->self);
}

void blink_stopHigh(Blink *self){
	self->control = blink_IDLE;
	self->slave->high(self->slave->self);
}

void blink_begin(Blink *self, iBlink *slave){
	self->slave = slave;
	ton_setInterval(&self->tmr, BLINK_INTERVAL_MS);
	ton_reset(&self->tmr);
	self->control = blink_IDLE;
}
