#include "blink.h"

static void blink_IDLE(Blink *item);
static void blink_LOW(Blink *item);
static void blink_HIGH(Blink *item);

static void blink_IDLE(Blink *item){
	;
}

static void blink_LOW(Blink *item){
	if(tonr(&item->tmr)){
		item->slave_low(item->slave);
		item->control = blink_HIGH;
	}
}

static void blink_HIGH(Blink *item){
	if(tonr(&item->tmr)){
		item->slave_high(item->slave);
		item->control = blink_LOW;
	}
}

void blink_start(Blink *item){
	ton_expire(&item->tmr);
	item->control = blink_HIGH;	
}

void blink_stop(Blink *item){
	item->control = blink_IDLE;
	item->slave_high(item->slave);
}

void blink_begin(Blink *item, void *slave, void (*slave_high)(void *), void (*slave_low)(void *)){
	item->slave = slave;
	item->slave_high = slave_high;
	item->slave_low = slave_low;
	ton_setInterval(&item->tmr, BLINK_INTERVAL_MS);
	ton_reset(&item->tmr);
	item->control = blink_IDLE;
}
