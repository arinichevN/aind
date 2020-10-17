#ifndef DISPLAY_BLINK_H
#define DISPLAY_BLINK_H

#include "../../util/ton.h"

#define BLINK_INTERVAL_MS		77

typedef struct blink_st Blink;
struct blink_st{
	void *slave;
	void (*slave_high)(void *);
	void (*slave_low)(void *);
	Ton tmr;
	void (*control) (Blink *);
};

extern void blink_start(Blink *item);

extern void blink_stop(Blink *item);

extern void blink_begin(Blink *item, void *slave, void (*slave_high)(void *), void (*slave_low)(void *));

#endif
