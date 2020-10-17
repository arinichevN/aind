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
#define MAX_FIRST_ALIGNMENT_LEFT_IND item->signs_count - j - 1
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

int display7_needUpdate(Display7 *item, const char *str, int alignment, int mode){
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen = item->buildBufOfStr(buf, DISPLAY_BUF_LEN, str);
	if(alignment != item->alignment || mode != item->mode || memcmp(buf, item->buf, sizeof buf) != 0){
		memcpy(item->buf, buf, sizeof buf);
		item->blen = blen;
		return 1;
	}
	return 0; 
}

static void display7_RUN(void *self);

#define MIN_FIRST_ALIGNMENT_RIGHT_IND	i + (item->signs_count - slen)
#define MIN_FIRST_ALIGNMENT_LEFT_IND	i
static void display7_setSignsMinFirst(Display7 *item, const char *str){
	size_t slen = strlen(str);
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(item->alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = MIN_FIRST_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = MIN_FIRST_ALIGNMENT_LEFT_IND;
				break;
		}
		item->signs[ind] = item->buf[i];
	}
}

static void display7_setSignsMaxFirst(Display7 *item, const char *str){
	size_t slen = strlen(str);
	size_t dslen = dstrlen(str);
	size_t j = 0;
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(item->alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = MAX_FIRST_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = MAX_FIRST_ALIGNMENT_LEFT_IND;
				break;
		}
		item->signs[ind] = item->buf[item->blen - 1 - i];
		j++;
	}
}

static void display7_printStrFit(Display7 *item, const char *str){
	uint8_t *signs = item->signs;
	memset(signs, 0, sizeof (*signs) * item->signs_count);
	item->setSigns(item, str);
	item->device->printSigns(item->device->self, signs);
	scroll_stop(&item->scroll);
}

static void display7_printStrScroll(Display7 *item, const char *str){
	scroll_start(&item->scroll, item->blen);
}

void display7_printStr(void *self, const char *str, int alignment){
	Display7 *item = (Display7 *) self;
	if(display7_needUpdate(item, str, alignment, DISPLAY_MODE_LIGHT)){
		blink_stop(&item->blink);
		item->alignment = alignment;
		item->mode = DISPLAY_MODE_LIGHT;
		size_t slen = strlen(str);
		if (slen <= item->signs_count) {
			display7_printStrFit(item, str);
		} else {
			display7_printStrScroll(item, str);
		}
	}
}

void display7_printBlinkStr(void *self, const char *str, int alignment){
	Display7 *item = (Display7 *) self;
	size_t slen = strlen(str);
	if(display7_needUpdate(item, str, alignment, DISPLAY_MODE_BLINK)){
		item->alignment = alignment;
		item->mode = DISPLAY_MODE_BLINK;
		if (slen <= item->signs_count) {
			display7_printStrFit(item, str);
		} else {
			display7_printStrScroll(item, str);
		}
		blink_start(&item->blink);
	}
}

static void display7_RUN(void *self){
	Display7 *item = (Display7 *) self;
	CONTROL(&item->blink);
	CONTROL(&item->scroll);
}

static void display7_blinkLow (void *self){
	Display7 *item = (Display7 *) self;
	item->device->clear(item->device->self);
}

static void display7_blinkHigh (void *self){
	Display7 *item = (Display7 *) self;
	item->device->printSigns(item->device->self, item->signs);
}

void display7_clear(void *self){
	Display7 *item = (Display7 *) self;
	item->device->clear(item->device->self);
}

void display7_control(void *self){
	Display7 *item = (Display7 *) self;
	item->control(item);
}

Display7 *display7_new(){
	size_t sz = sizeof (Display7);
	Display7 *out = (Display7 *) malloc(sz);
	if(out == NULL){ printdln("display7_new: failed");}
	memset(out, 0, sz);
	printd("display7_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int display7_beginSigns(Display7 *item, size_t signs_count){
	item->signs = (uint8_t *) malloc(sizeof (*item->signs) * signs_count);
	if(item->signs == NULL){
		printdln("display7: malloc failed for signs");
		return 0;
	}
	item->signs_count = signs_count;
	return 1;
}

void display7_setParam(Display7 *item, int device_kind, i7Segment *device, size_t (*buildBufOfStr)(uint8_t *, size_t, const char *), void (*setSigns)(Display7 *, const char *)){
	item->device = device;
	item->buildBufOfStr = buildBufOfStr;
	item->setSigns = setSigns;
}

void display7_free(void *self){
	Display7 *item = (Display7 *) self;
	free(item->device->self);
	free(item->signs);
}

void display7_buildInterfaces(Display7 *item){
	item->im_blink.self = item;
	item->im_blink.high = display7_blinkHigh;
	item->im_blink.low = display7_blinkLow;
	item->im_display.self = item;
	item->im_display.clear = display7_clear;
	item->im_display.control = display7_control;
	item->im_display.free = display7_free;
	item->im_display.printStr = display7_printStr;
	item->im_display.printBlinkStr = display7_printBlinkStr;
}

int display7_begin(Display7 *item, int device_kind, int p1, int p2, int p3){
	display7_buildInterfaces(item);
	int scroll_kind = SCROLL_KIND_NONE;
	iScroll *i_scroll = NULL;
	switch(device_kind){
		case DEVICE_KIND_TM1637:{
				TM1637 *device = tm1637_new();
				if(device == NULL){
					return 0;
				}
				tm1637_begin(device, p1, p2);
				int r = display7_beginSigns(item, TM1637_SIGNS_COUNT);
				if(!r){
					return 0;
				}
				display7_setParam(item, device_kind, &device->im_7segment, display7_buildBufOfStrMinFirst, display7_setSignsMinFirst);
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
				int r = display7_beginSigns(item, MAX7219_SIGNS_COUNT);
				if(!r){
					return 0;
				}
				display7_setParam(item, device_kind, &device->im_7segment, display7_buildBufOfStrMaxFirst, display7_setSignsMaxFirst);
				scroll_kind = SCROLL_KIND_MAX_FIRST;
				i_scroll = &device->im_scroll;
			}
			break;
		default: 
			return 0;
	}
	blink_begin(&item->blink, &item->im_blink);
	scroll_begin(&item->scroll, scroll_kind, i_scroll, item->signs, item->signs_count, item->buf);
	item->alignment = DISPLAY_ALIGNMENT_LEFT + DISPLAY_ALIGNMENT_RIGHT;
	item->mode = DISPLAY_MODE_LIGHT + DISPLAY_MODE_BLINK;
	item->control = display7_RUN;
	
	return 1;
}


