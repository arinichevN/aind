#include "scroll.h"

static void scroll_RUN(Scroll *self);

static void scroll_IDLE(Scroll *self){
	;
}

static void scroll_RESET(Scroll *self){
	memset(self->data->signs, 0, sizeof (*self->data->signs) * self->data->signs_count);
	ton_expire(&self->tmr);
	self->i = 0;
	self->control = scroll_RUN;
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

static void scroll_RUN(Scroll *self){
	if(tonr(&self->tmr)){
		if(self->i >= self->blen + self->data->signs_count){
			self->control = scroll_RESET;
			return;
		}
		self->scroll_func(self->data->signs, self->data->signs_count, self->data->buf, self->blen, self->i);
		self->slave->printSigns(self->slave->self, self->data->signs);
		self->i++;
	}
}

void scroll_start(Scroll *self, size_t blen){
	self->blen = blen;
	self->control = scroll_RESET;
}

void scroll_stop(Scroll *self){
	self->control = scroll_IDLE;
}

void scroll_begin(Scroll *self, int kind, iScrollSlave *slave, iScrollData *data){
	switch(kind){
		case SCROLL_KIND_MIN_FIRST: self->scroll_func = scroll_scrollMinFirst; break;
		case SCROLL_KIND_MAX_FIRST: self->scroll_func = scroll_scrollMaxFirst; break;
		default: printdln("scroll: bad kind"); self->scroll_func = scroll_scrollNone;
	}
	ton_setInterval(&self->tmr, DISPLAY_SCROLL_INTERVAL_MS);
	ton_reset(&self->tmr);
	self->slave = slave;
	self->data = data;
	self->control = scroll_IDLE;
}

