#ifndef DISPLAY_SCROLL_H
#define DISPLAY_SCROLL_H

#include "../../lib/ton.h"
#include "../interface/iScrollSlave.h"
#include "../interface/iScrollData.h"

#define DISPLAY_SCROLL_INTERVAL_MS		300

enum{
	SCROLL_KIND_NONE,
	SCROLL_KIND_MIN_FIRST,
	SCROLL_KIND_MAX_FIRST
};

typedef struct scroll_st Scroll;
struct scroll_st{
	iScrollSlave *slave;
	iScrollData *data;
	void (*scroll_func) (uint8_t *signs, size_t signs_count, uint8_t *buf, size_t blen, size_t i);
	Ton tmr;
	size_t blen;
	size_t i;
	void (*control) (Scroll *);
};

extern void scroll_start(Scroll *item, size_t blen);

extern void scroll_stop(Scroll *item);

extern void scroll_begin(Scroll *item, int kind, iScrollSlave *slave, iScrollData *data);
	
#endif
