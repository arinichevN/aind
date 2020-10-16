#include "blink.h"

static void blink_IDLE(Blink *item);
static void blink_LOW(Blink *item);
static void blink_HIGH(Blink *item);

static void blink_IDLE(Blink *item){
	;
}

static void blink_LOW(Blink *item){
	if(tonr(&item->tmr)){
		item->functionLow(item->slave);
		item->control = blink_HIGH;
	}
}

static void blink_HIGH(Blink *item){
	if(tonr(&item->tmr)){
		item->functionHigh(item->slave);
		item->control = blink_LOW;
	}
}

void blink_start(Blink *item){
	ton_expire(&item->tmr);
	item->control = blink_HIGH;	
}

void blink_stop(Blink *item){
	item->control = blink_IDLE;
	item->functionHigh(item->slave);
}

void blink_begin(Blink *item, void *slave, void (*functionHigh)(void *), void (*functionLow)(void *)){
	item->slave = slave;
	item->functionHigh = functionHigh;
	item->functionLow = functionLow;
	ton_setInterval(&item->tmr, BLINK_INTERVAL_MS);
	ton_reset(&item->tmr);
	item->control = blink_IDLE;
}
