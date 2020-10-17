#include "MAX7219.h"

static void max7219_write(MAX7219 *item, volatile uint8_t reg, volatile uint8_t data) {
	digitalWrite(item->cs, LOW);
	shiftOut(item->din, item->clk, MSBFIRST, reg);
	shiftOut(item->din, item->clk, MSBFIRST, data);
	digitalWrite(item->cs, HIGH);
} 

void max7219_clear(void *self) {
	MAX7219 *item = (MAX7219 *) self;
	for(size_t i = MAX7219_REG_FIRST_DIGIT; i < MAX7219_REG_FIRST_DIGIT + MAX7219_SIGNS_COUNT; i++) {
		max7219_write(item, i, 0x00);
	}
}

void max7219_setBrightness (MAX7219 *item, uint8_t brightness){
	max7219_write(item, MAX7219_REG_INTENSITY, brightness & 0x0f);
}

void max7219_printSigns(void *self, const uint8_t *signs){
	MAX7219 *item = (MAX7219 *) self;
	for (size_t i = 0; i < MAX7219_SIGNS_COUNT; i++){
		max7219_write(item, MAX7219_REG_FIRST_DIGIT + i, signs[i]); //printd(signs[i]); printd(" ");
	}
	//printdln(" ");
}

MAX7219 *max7219_new(){
	size_t sz = sizeof (MAX7219);
	MAX7219 *out = (MAX7219 *) malloc(sz);
	if(out == NULL){ printdln("max7219_new: failed");}
	memset(out, 0, sz);
	printd("max7219_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void max7219_buildInterfaces(MAX7219 *item){
	item->im_7segment.self = item;
	item->im_7segment.clear = max7219_clear;
	item->im_7segment.printSigns = max7219_printSigns;
	item->im_scroll.self = item;
	item->im_scroll.printSigns = max7219_printSigns;
}

void max7219_begin(MAX7219 *item, int din, int clk, int cs){
	max7219_buildInterfaces(item);
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
}
