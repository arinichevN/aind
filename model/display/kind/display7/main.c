#include "main.h"

static size_t display7_buildBufOfStrMinFirst(uint8_t *buf, size_t blen, const char *str){
	memset(buf, 0, sizeof (*buf) * blen);
	size_t slen = strlen(str);
	size_t i = 0;
	for (i = 0; i < slen; i++) {
		if(i >= blen) break;
		buf[i] = encodeASCII(str[i]);
	}
	return i;
}

#define MAX_FIRST_ALIGNMENT_RIGHT_IND dslen - j - 1
#define MAX_FIRST_ALIGNMENT_LEFT_IND self->signs_count - j - 1
static size_t display7_buildBufOfStrMaxFirst(uint8_t *buf, size_t blen, const char *str){
	memset(buf, 0, sizeof (*buf) * blen);
	size_t slen = strlen(str);
	size_t dslen = dstrlen(str);
	size_t j = 0;
	for (size_t i = 0; i < slen; i++) {
		size_t ind = MAX_FIRST_ALIGNMENT_RIGHT_IND;
		if(ind >= blen) break;
		buf[ind] = encodeSegmentFromStrSwap(str, slen, &i);
		j++;
	}
	return j;
}

int display7_needUpdate(Display7 *self, const char *str, int alignment, int mode){
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen = self->buildBufOfStr(buf, DISPLAY_BUF_LEN, str);
	if(alignment != self->alignment || mode != self->mode || memcmp(buf, self->buf, sizeof buf) != 0){
		memcpy(self->buf, buf, sizeof buf);
		self->blen = blen;
		return 1;
	}
	return 0; 
}

static void display7_RUN(void *self);

#define MIN_FIRST_ALIGNMENT_RIGHT_IND	i + (self->signs_count - slen)
#define MIN_FIRST_ALIGNMENT_LEFT_IND	i
static void display7_setSignsMinFirst(Display7 *self, const char *str){
	size_t slen = strlen(str);
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(self->alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = MIN_FIRST_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = MIN_FIRST_ALIGNMENT_LEFT_IND;
				break;
		}
		self->signs[ind] = self->buf[i];
	}
}

static void display7_setSignsMaxFirst(Display7 *self, const char *str){
	size_t slen = strlen(str);
	size_t dslen = dstrlen(str);
	size_t j = 0;
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(self->alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = MAX_FIRST_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = MAX_FIRST_ALIGNMENT_LEFT_IND;
				break;
		}
		self->signs[ind] = self->buf[self->blen - 1 - i];
		j++;
	}
}

static void display7_printStrFit(Display7 *self, const char *str){
	uint8_t *signs = self->signs;
	memset(signs, 0, sizeof (*signs) * self->signs_count);
	self->setSigns(self, str);
	self->device->printSigns(self->device->self, signs);
	scroll_stop(&self->scroll);
}

static void display7_printStrScroll(Display7 *self, const char *str){
	scroll_start(&self->scroll, self->blen);
}

void display7_printStr(void *vself, const char *str, int alignment){
	Display7 *self = (Display7 *) vself;
	if(display7_needUpdate(self, str, alignment, DISPLAY_MODE_LIGHT)){
		blink_stopHigh(&self->blink);
		self->alignment = alignment;
		self->mode = DISPLAY_MODE_LIGHT;
		size_t slen = strlen(str);
		if (slen <= self->signs_count) {
			display7_printStrFit(self, str);
		} else {
			display7_printStrScroll(self, str);
		}
	}
}

void display7_printBlinkStr(void *vself, const char *str, int alignment){
	Display7 *self = (Display7 *) vself;
	size_t slen = strlen(str);
	if(display7_needUpdate(self, str, alignment, DISPLAY_MODE_BLINK)){
		self->alignment = alignment;
		self->mode = DISPLAY_MODE_BLINK;
		if (slen <= self->signs_count) {
			display7_printStrFit(self, str);
		} else {
			display7_printStrScroll(self, str);
		}
		blink_start(&self->blink);
	}
}

static void display7_RUN(void *vself){
	Display7 *self = (Display7 *) vself;
	CONTROL(&self->blink);
	CONTROL(&self->scroll);
}

static void display7_blinkLow (void *vself){
	Display7 *self = (Display7 *) vself;
	self->device->clear(self->device->self);
}

static void display7_blinkHigh (void *vself){
	Display7 *self = (Display7 *) vself;
	self->device->printSigns(self->device->self, self->signs);
}

void display7_clear(void *vself){
	Display7 *self = (Display7 *) vself;
	self->device->clear(self->device->self);
}

void display7_control(void *vself){
	Display7 *self = (Display7 *) vself;
	self->control(self);
}

Display7 *display7_new(){
	size_t sz = sizeof (Display7);
	Display7 *out = (Display7 *) malloc(sz);
	if(out == NULL){ printdln("display7_new: failed");}
	memset(out, 0, sz);
	printd("display7_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int display7_beginSigns(Display7 *self, size_t signs_count){
	self->signs = (uint8_t *) malloc(sizeof (*self->signs) * signs_count);
	if(self->signs == NULL){
		printdln("display7: malloc failed for signs");
		return 0;
	}
	self->signs_count = signs_count;
	return 1;
}

void display7_setParam(Display7 *self, int device_kind, i7Segment *device, size_t (*buildBufOfStr)(uint8_t *, size_t, const char *), void (*setSigns)(Display7 *, const char *)){
	self->device = device;
	self->buildBufOfStr = buildBufOfStr;
	self->setSigns = setSigns;
}

void display7_free(void *vself){
	Display7 *self = (Display7 *) vself;
	free(self->device->self);
	free(self->signs);
}

void display7_buildInterfaces(Display7 *self){
	self->im_blink.self = self;
	self->im_blink.high = display7_blinkHigh;
	self->im_blink.low = display7_blinkLow;
	self->im_display.self = self;
	self->im_display.clear = display7_clear;
	self->im_display.control = display7_control;
	self->im_display.free = display7_free;
	self->im_display.printStr = display7_printStr;
	self->im_display.printBlinkStr = display7_printBlinkStr;
	self->im_scroll_data.buf = self->buf;
	self->im_scroll_data.signs = self->signs;
	self->im_scroll_data.signs_count = self->signs_count;
	
}

void display7_buildScrollInterface(Display7 *self){
	self->im_scroll_data.buf = self->buf;
	self->im_scroll_data.signs = self->signs;
	self->im_scroll_data.signs_count = self->signs_count;
	
}

int display7_begin(Display7 *self, int device_kind, int p1, int p2, int p3){
	display7_buildInterfaces(self);
	int scroll_kind = SCROLL_KIND_NONE;
	iScrollSlave *i_scroll = NULL;
	switch(device_kind){
		case DEVICE_KIND_TM1637:{
				TM1637 *device = tm1637_new();
				if(device == NULL){
					return 0;
				}
				tm1637_begin(device, p1, p2);
				int r = display7_beginSigns(self, TM1637_SIGNS_COUNT);
				if(!r){
					return 0;
				}
				display7_setParam(self, device_kind, &device->im_7segment, display7_buildBufOfStrMinFirst, display7_setSignsMinFirst);
				scroll_kind = SCROLL_KIND_MIN_FIRST;
				i_scroll = &device->im_scroll;
			}
			break;
		case DEVICE_KIND_MAX7219:{
				MAX7219 *device = max7219_new();
				if(device == NULL){
					return 0;
				}
				max7219_begin(device, p1, p2, p3);
				int r = display7_beginSigns(self, MAX7219_SIGNS_COUNT);
				if(!r){
					return 0;
				}
				display7_setParam(self, device_kind, &device->im_7segment, display7_buildBufOfStrMaxFirst, display7_setSignsMaxFirst);
				scroll_kind = SCROLL_KIND_MAX_FIRST;
				i_scroll = &device->im_scroll;
			}
			break;
		default: 
			return 0;
	}
	display7_buildScrollInterface(self);
	blink_begin(&self->blink, &self->im_blink);
	scroll_begin(&self->scroll, scroll_kind, i_scroll, &self->im_scroll_data);
	self->alignment = DISPLAY_ALIGNMENT_LEFT + DISPLAY_ALIGNMENT_RIGHT;
	self->mode = DISPLAY_MODE_LIGHT + DISPLAY_MODE_BLINK;
	self->control = display7_RUN;
	
	return 1;
}


