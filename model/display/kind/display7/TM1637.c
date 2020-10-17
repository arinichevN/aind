#include "TM1637.h"

void tm1637_setBrightness(TM1637 *item, uint8_t brightness) {
	item->brightness = (brightness & 0x07) | 0x08;
}

static void tm1637_start(TM1637 *item){
	pinMode(item->dio, OUTPUT);
	TM1637_DELAY
}

static void tm1637_stop(TM1637 *item){
	pinMode(item->dio, OUTPUT);
	TM1637_DELAY
	pinMode(item->clk, INPUT);
	TM1637_DELAY
	pinMode(item->dio, INPUT);
	TM1637_DELAY
}

static int tm1637_write(TM1637 *item, uint8_t v){
	uint8_t data = v;
	
	// 8 Data Bits
	for(uint8_t i = 0; i < 8; i++) {
		// CLK low
		pinMode(item->clk, OUTPUT);
		TM1637_DELAY
		
		// Set data bit
		if (data & 0x01) pinMode(item->dio, INPUT); 
		else pinMode(item->dio, OUTPUT);
		
		TM1637_DELAY
		
		// CLK high
		pinMode(item->clk, INPUT);
		TM1637_DELAY
		data = data >> 1;
	}
	
	// Wait for acknowledge
	// CLK to zero
	pinMode(item->clk, OUTPUT);
	pinMode(item->dio, INPUT);
	TM1637_DELAY
	
	// CLK to high
	pinMode(item->clk, INPUT);
	TM1637_DELAY
	int ack = digitalRead(item->dio);
	if (ack == 0) pinMode(item->dio, OUTPUT);
	
	TM1637_DELAY
	pinMode(item->clk, OUTPUT);
	TM1637_DELAY
	
	return ack;
}

void tm1637_printSigns(void *self, const uint8_t *signs){
	TM1637 *item = (TM1637 *) self;
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM1);
	tm1637_stop(item);

	//first digit address
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (size_t i=0; i < TM1637_SIGNS_COUNT; i++)
	  tm1637_write(item, signs[i]);

	tm1637_stop(item);

	//brightness
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM3 + (item->brightness & 0x0f));
	tm1637_stop(item);
}

void tm1637_clear(void *self){
	TM1637 *item = (TM1637 *) self;
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM1);
	tm1637_stop(item);

	//first digit address
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (size_t i=0; i < TM1637_SIGNS_COUNT; i++){
		tm1637_write(item, 0x00);
	}
	tm1637_stop(item);
}

TM1637 *tm1637_new(){
	size_t sz = sizeof (TM1637);
	TM1637 *out = (TM1637 *) malloc(sz);
	if(out == NULL){ printdln("tm1637_new: failed");}
	memset(out, 0, sz);
	printd("tm1637_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void tm1637_buildInterfaces(TM1637 *item){
	item->im_7segment.self = item;
	item->im_7segment.clear = tm1637_clear;
	item->im_7segment.printSigns = tm1637_printSigns;
	item->im_scroll.self = item;
	item->im_scroll.printSigns = tm1637_printSigns;
}

void tm1637_begin(TM1637 *item, int dio, int clk){
	tm1637_buildInterfaces(item);
	item->dio = dio;
	item->clk = clk;
	pinMode(item->clk, INPUT);
	pinMode(item->dio,INPUT);
	digitalWrite(item->clk, LOW);
	digitalWrite(item->dio, LOW);
	tm1637_setBrightness(item, TM1637_BRIGHTNESS);
}
