#include "MAX7219.h"

static void max7219_write(MAX7219 *item, volatile uint8_t reg, volatile uint8_t data) {
	digitalWrite(item->cs, LOW);
    shiftOut(item->din, item->clk, MSBFIRST, reg);
    shiftOut(item->din, item->clk, MSBFIRST, data);
    digitalWrite(item->cs, HIGH);
} 

void max7219_clear(void *device) {
	MAX7219 *item = (MAX7219 *) device;
    for(int i = MAX7219_REG_FIRST_DIGIT; i < MAX7219_REG_FIRST_DIGIT + MAX7219_SIGNS_COUNT; i++) {
        max7219_write(item, i, 0x00);
    }
}

void max7219_setBrightness (MAX7219 *item, uint8_t brightness){
	max7219_write(item, MAX7219_REG_INTENSITY, brightness & 0x0f);
}

static void max7219_setSigns(MAX7219 *item, uint8_t signs[]){
	for (uint8_t i = 0; i < MAX7219_SIGNS_COUNT; i++){
		max7219_write(item, MAX7219_REG_FIRST_DIGIT + i, signs[i]); //printd(signs[i]); printd(" ");
	}
	//printdln(" ");
}

#define MAX7219_ALIGNMENT_RIGHT_IND dslen - j - 1
#define MAX7219_ALIGNMENT_LEFT_IND MAX7219_SIGNS_COUNT - j - 1

static size_t max7219_buildBufOfStr(uint8_t buf[], size_t blen, const char *str){
	memset(buf, 0, sizeof (*buf) * blen);
	size_t slen = strlen(str);
	size_t dslen = dstrlen(str);
	size_t j = 0;
	for (size_t i = 0; i < slen; i++) {
		size_t ind = MAX7219_ALIGNMENT_RIGHT_IND;
		if(ind >= blen) break;
		buf[ind] = encodeSegmentFromStrSwap(str, slen, &i);
		j++;
	}
	return j;
}

int max7219_needUpdate(MAX7219 *item, const char *str, int alignment){
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen = max7219_buildBufOfStr(buf, DISPLAY_BUF_LEN, str);
	if(alignment != item->alignment || memcmp(buf, item->buf, sizeof buf) != 0){
		memcpy(item->buf, buf, sizeof buf);
		item->blen = blen;
		return 1;
	}
	return 0; 
}

static void max7219_IDLE(void *device);
static void max7219_SCROLL_START(void *device);
static void max7219_SCROLL(void *device);

static void max7219_printStrFit(MAX7219 *item, const char *str, int alignment){
	uint8_t *signs = item->signs;
	memset(signs, 0, sizeof (*signs) * MAX7219_SIGNS_COUNT);
	size_t slen = strlen(str);
	size_t dslen = dstrlen(str);
	size_t j = 0;
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = MAX7219_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = MAX7219_ALIGNMENT_LEFT_IND;
				break;
		}
		//signs[ind] = encodeSegmentFromStrSwap(str, slen, &i);
		signs[ind] = item->buf[item->blen - 1 - i];
		j++;

	}
	max7219_setSigns(item, signs);
	item->control = max7219_IDLE;
}

static void max7219_printStrScroll(MAX7219 *item, const char *str){
	item->control = max7219_SCROLL_START;
}

void max7219_printStr(void *device, const char *str, int alignment){
	MAX7219 *item = (MAX7219 *) device;
	if(max7219_needUpdate(item, str, alignment)){
		blink_stop(&item->blink);
		item->alignment = alignment;
		size_t slen = strlen(str);
		if (slen <= MAX7219_SIGNS_COUNT) {
			max7219_printStrFit(item, str, alignment);
		} else {
			max7219_printStrScroll(item, str);
		}
	}
}

void max7219_printBlinkStr(void *device, const char *str, int alignment){
	MAX7219 *item = (MAX7219 *) device;
	size_t slen = strlen(str);
	if(max7219_needUpdate(item, str, alignment)){
		item->alignment = alignment;
		if (slen <= MAX7219_SIGNS_COUNT) {
			max7219_printStrFit(item, str, alignment);
		} else {
			max7219_printStrScroll(item, str);
		}
		blink_start(&item->blink);
	}
}

static void max7219_IDLE(void *device){
	MAX7219 *item = (MAX7219 *)device;
	Blink *blink = &item->blink;
	CONTROL(blink);
}

static void max7219_SCROLL(void *device){
	MAX7219 *item = (MAX7219 *)device;
	if(tonr(&item->tmr)){
		if(item->i >= item->blen + MAX7219_SIGNS_COUNT){
			item->control = max7219_SCROLL_START;
			return;
		}
		uint8_t *signs = item->signs;
		for (size_t j = MAX7219_SIGNS_COUNT - 1; j > 0 ; j--) {
			signs[j] = signs[j-1];
		}
		uint8_t c = 0;
		if(item->i < item->blen){
			c = item->buf[item->blen - 1 - item->i];
		}
		signs[0] = c;
		max7219_setSigns(item, signs);
		item->i++;
	}
	Blink *blink = &item->blink;
	CONTROL(blink);
}

static void max7219_SCROLL_START(void *device){
	MAX7219 *item = (MAX7219 *)device;
	memset(item->signs, 0, sizeof (*item->signs) * MAX7219_SIGNS_COUNT);
	ton_expire(&item->tmr);
	item->i = 0;
	item->control = max7219_SCROLL;
}

static void max7219_blinkLow (void *device){
	max7219_clear(device);
}

static void max7219_blinkHigh (void *device){
	MAX7219 *item = (MAX7219 *) device;
	max7219_setSigns(item, item->signs);
}

void max7219_control(void *device){
	MAX7219 *item = (MAX7219 *) device;
	item->control(item);
}

MAX7219 *max7219_new(){
	size_t sz = sizeof (MAX7219);
	MAX7219 *out = (MAX7219 *) malloc(sz);
	if(out == NULL){ printdln("max7219_new: failed");}
	printd("max7219_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void max7219_begin(MAX7219 *item, int din, int clk, int cs){
	item->din = din;
	item->clk = clk;
	item->cs = cs;
	pinMode(item->din, OUTPUT);
	pinMode(item->clk, OUTPUT);
	pinMode(item->cs, OUTPUT);
	digitalWrite(item->cs, HIGH);
	max7219_write(item, MAX7219_REG_SCAN_LIMIT, MAX7219_SCAN_LIMIT);
	max7219_write(item, MAX7219_REG_DECODE_MODE, MAX7219_DECODE_MODE);
	max7219_write(item, MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_STOP);
	max7219_write(item, MAX7219_REG_DISPLAY_TEST, MAX7219_DISPLAY_TEST_STOP);
	max7219_clear((void *) item);
	max7219_setBrightness(item, MAX7219_BRIGHTNESS);
	ton_setInterval(&item->tmr, DISPLAY_SCROLL_INTERVAL_MS);
	ton_reset(&item->tmr);
	blink_begin(&item->blink, item, max7219_blinkHigh, max7219_blinkLow);
	item->alignment = DISPLAY_ALIGNMENT_LEFT + DISPLAY_ALIGNMENT_RIGHT;
	item->control = max7219_IDLE;
}
