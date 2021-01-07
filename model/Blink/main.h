#ifndef DISPLAY_BLINK_H
#define DISPLAY_BLINK_H

#include "../../lib/ton.h"
#include "../interface/iBlink.h"

#define BLINK_INTERVAL_MS		77

typedef struct blink_st Blink;
struct blink_st{
	iBlink *slave;
	Ton tmr;
	void (*control) (Blink *);
};

extern void blink_start(Blink *self);

extern void blink_stopLow(Blink *self);

extern void blink_stopHigh(Blink *self);

extern void blink_begin(Blink *self, iBlink *slave);

#endif
