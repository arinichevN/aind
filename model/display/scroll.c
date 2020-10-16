#include "scroll.h"

static void scroll_RUN(Scroll *item);

static void scroll_IDLE(Scroll *item){
	;
}

static void scroll_RESET(Scroll *item){
	memset(item->signs, 0, sizeof (*item->signs) * item->signs_count);
	ton_expire(&item->tmr);
	item->i = 0;
	item->control = scroll_RUN;
}

static void scroll_scrollNone(uint8_t *signs, size_t signs_count, uint8_t *buf, size_t blen, size_t i){
	;
}

static void scroll_scrollMinFirst(uint8_t *signs, size_t signs_count, uint8_t *buf, size_t blen, size_t i){
	for (size_t j = 0; j < signs_count - 1; j++) {
		signs[j] = signs[j + 1];
	}
	uint8_t c = 0;
	if(i < blen){
		c = buf[i];
	}
	signs[signs_count - 1] = c;
}

static void scroll_scrollMaxFirst(uint8_t *signs, size_t signs_count, uint8_t *buf, size_t blen, size_t i){
	for (size_t j = signs_count - 1; j > 0 ; j--) {
		signs[j] = signs[j - 1];
	}
	uint8_t c = 0;
	if(i < blen){
		c = buf[blen - 1 - i];
	}
	signs[0] = c;
}

static void scroll_RUN(Scroll *item){
	if(tonr(&item->tmr)){
		if(item->i >= item->blen + item->signs_count){
			item->control = scroll_RESET;
			return;
		}
		item->scroll_func(item->signs, item->signs_count, item->buf, item->blen, item->i);
		item->slave_setSigns(item->slave, item->signs);
		item->i++;
	}
}

void scroll_start(Scroll *item, size_t blen){
	item->blen = blen;
	item->control = scroll_RESET;
}

void scroll_stop(Scroll *item){
	item->control = scroll_IDLE;
}

void scroll_begin(Scroll *item, int kind, void *slave, void(*setSigns)(void *, const uint8_t *), uint8_t *signs, size_t signs_count, uint8_t *buf){
	switch(kind){
		case SCROLL_KIND_MIN_FIRST: item->scroll_func = scroll_scrollMinFirst; break;
		case SCROLL_KIND_MAX_FIRST: item->scroll_func = scroll_scrollMaxFirst; break;
		default: printdln("scroll: bad kind"); item->scroll_func = scroll_scrollNone;
	}
	ton_setInterval(&item->tmr, DISPLAY_SCROLL_INTERVAL_MS);
	ton_reset(&item->tmr);
	item->slave = slave;
	item->slave_setSigns = setSigns;
	item->signs = signs;
	item->signs_count = signs_count;
	item->buf = buf;
	item->control = scroll_IDLE;
}

