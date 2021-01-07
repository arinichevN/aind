#include "main.h"

void tm1637_setBrightness(TM1637 *self, uint8_t brightness) {
	self->brightness = (brightness & 0x07) | 0x08;
}

static void tm1637_start(TM1637 *self){
	pinMode(self->dio, OUTPUT);
	TM1637_DELAY
}

static void tm1637_stop(TM1637 *self){
	pinMode(self->dio, OUTPUT);
	TM1637_DELAY
	pinMode(self->clk, INPUT);
	TM1637_DELAY
	pinMode(self->dio, INPUT);
	TM1637_DELAY
}

static int tm1637_write(TM1637 *self, uint8_t v){
	uint8_t data = v;
	
	// 8 Data Bits
	for(uint8_t i = 0; i < 8; i++) {
		// CLK low
		pinMode(self->clk, OUTPUT);
		TM1637_DELAY
		
		// Set data bit
		if (data & 0x01) pinMode(self->dio, INPUT); 
		else pinMode(self->dio, OUTPUT);
		
		TM1637_DELAY
		
		// CLK high
		pinMode(self->clk, INPUT);
		TM1637_DELAY
		data = data >> 1;
	}
	
	// Wait for acknowledge
	// CLK to zero
	pinMode(self->clk, OUTPUT);
	pinMode(self->dio, INPUT);
	TM1637_DELAY
	
	// CLK to high
	pinMode(self->clk, INPUT);
	TM1637_DELAY
	int ack = digitalRead(self->dio);
	if (ack == 0) pinMode(self->dio, OUTPUT);
	
	TM1637_DELAY
	pinMode(self->clk, OUTPUT);
	TM1637_DELAY
	
	return ack;
}

void tm1637_printSigns(void *vself, const uint8_t *signs){
	TM1637 *self = (TM1637 *) vself;
	tm1637_start(self);
	tm1637_write(self, TM1637_I2C_COMM1);
	tm1637_stop(self);

	//first digit address
	tm1637_start(self);
	tm1637_write(self, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (size_t i=0; i < TM1637_SIGNS_COUNT; i++)
	  tm1637_write(self, signs[i]);

	tm1637_stop(self);

	//brightness
	tm1637_start(self);
	tm1637_write(self, TM1637_I2C_COMM3 + (self->brightness & 0x0f));
	tm1637_stop(self);
}

void tm1637_clear(void *vself){
	TM1637 *self = (TM1637 *) vself;
	tm1637_start(self);
	tm1637_write(self, TM1637_I2C_COMM1);
	tm1637_stop(self);

	//first digit address
	tm1637_start(self);
	tm1637_write(self, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (size_t i=0; i < TM1637_SIGNS_COUNT; i++){
		tm1637_write(self, 0x00);
	}
	tm1637_stop(self);
}

TM1637 *tm1637_new(){
	size_t sz = sizeof (TM1637);
	TM1637 *out = (TM1637 *) malloc(sz);
	if(out == NULL){ printdln("tm1637_new: failed");}
	memset(out, 0, sz);
	printd("tm1637_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void tm1637_buildInterfaces(TM1637 *self){
	self->im_7segment.self = self;
	self->im_7segment.clear = tm1637_clear;
	self->im_7segment.printSigns = tm1637_printSigns;
	self->im_scroll.self = self;
	self->im_scroll.printSigns = tm1637_printSigns;
}

void tm1637_begin(TM1637 *self, int dio, int clk){
	tm1637_buildInterfaces(self);
	self->dio = dio;
	self->clk = clk;
	pinMode(self->clk, INPUT);
	pinMode(self->dio,INPUT);
	digitalWrite(self->clk, LOW);
	digitalWrite(self->dio, LOW);
	tm1637_setBrightness(self, TM1637_BRIGHTNESS);
}
