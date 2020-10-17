#ifndef DISPLAY_SCROLL_H
#define DISPLAY_SCROLL_H

#include "../../util/ton.h"

#define DISPLAY_SCROLL_INTERVAL_MS		300

enum{
	SCROLL_KIND_NONE,
	SCROLL_KIND_MIN_FIRST,
	SCROLL_KIND_MAX_FIRST
};

typedef struct scroll_st Scroll;
struct scroll_st{
	void *slave;
	uint8_t *signs;
	uint8_t *buf;
	void (*slave_setSigns) (void *, const uint8_t *signs);
	void (*scroll_func) (uint8_t *signs, size_t signs_count, uint8_t *buf, size_t blen, size_t i);
	Ton tmr;
	size_t blen;
	size_t signs_count;
	size_t i;
	void (*control) (Scroll *);
};

extern void scroll_start(Scroll *item, size_t blen);

extern void scroll_stop(Scroll *item);

extern void scroll_begin(Scroll *item, int kind, void *slave, void(*setSigns)(void *, const uint8_t *), uint8_t *signs, size_t signs_count, uint8_t *buf);
	
#endif
