#include "TM1637.h"

static void tm1637_IDLE(void *device);
static void tm1637_SCROLL_START(void *device);
static void tm1637_SCROLL(void *device);

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

void tm1637_setSigns(TM1637 *item, const uint8_t signs[]){
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM1);
	tm1637_stop(item);

	//first digit address
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (uint8_t i=0; i < TM1637_SIGNS_COUNT; i++)
	  tm1637_write(item, signs[i]);

	tm1637_stop(item);

	//brightness
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM3 + (item->brightness & 0x0f));
	tm1637_stop(item);
}

static size_t tm1637_buildBufOfStr(uint8_t buf[], size_t blen, const char *str){
	memset(buf, 0, sizeof (*buf) * blen);
	size_t slen = strlen(str);
	size_t i = 0;
	for (i = 0; i < slen; i++) {
		if(i >= blen) break;
		buf[i] = encodeASCII(str[i]);
	}
	return i;
}

int tm1637_needUpdate(TM1637 *item, const char *str, int alignment){
	uint8_t buf[DISPLAY_BUF_LEN];
	size_t blen = tm1637_buildBufOfStr(buf, DISPLAY_BUF_LEN, str);
	if(alignment != item->alignment || memcmp(buf, item->buf, sizeof buf) != 0){
		memcpy(item->buf, buf, sizeof buf);
		item->blen = blen;
		return 1;
	}
	return 0; 
}

#define TM1637_ALIGNMENT_RIGHT_IND	i + (TM1637_SIGNS_COUNT - slen)
#define TM1637_ALIGNMENT_LEFT_IND	i

static void tm1637_printStrFit(TM1637 *item, const char *str){
	uint8_t *signs = item->signs;
	memset(signs, 0, sizeof (*signs) * TM1637_SIGNS_COUNT);
	size_t slen = strlen(str);
	for (size_t i = 0; i < slen; i++) {
		size_t ind;
		switch(item->alignment){
			case DISPLAY_ALIGNMENT_RIGHT:
				ind = TM1637_ALIGNMENT_RIGHT_IND;
				break;
			case DISPLAY_ALIGNMENT_LEFT:
			default:
				ind = TM1637_ALIGNMENT_LEFT_IND;
				break;
		}
		//signs[ind] = encodeASCII(str[i]);
		signs[ind] = item->buf[i];
	}
	tm1637_setSigns(item, signs);
	item->control = tm1637_IDLE;
}

static void tm1637_printStrScroll(TM1637 *item, const char *str){
	item->control = tm1637_SCROLL_START;
}

void tm1637_printStr(void *device, const char *str, int alignment){
	TM1637 *item = (TM1637 *) device;
	if(tm1637_needUpdate(item, str, alignment)){
		blink_stop(&item->blink);
		item->alignment = alignment;
		size_t slen = strlen(str);
		if (slen <= TM1637_SIGNS_COUNT) {
			tm1637_printStrFit(item, str);
		} else {
			tm1637_printStrScroll(item, str);
		}
	}
}

void tm1637_printBlinkStr(void *device, const char *str, int alignment){
	TM1637 *item = (TM1637 *) device;
	if(tm1637_needUpdate(item, str, alignment)){
		item->alignment = alignment;
		size_t slen = strlen(str);
		if (slen <= TM1637_SIGNS_COUNT) {
			tm1637_printStrFit(item, str);
		} else {
			tm1637_printStrScroll(item, str);
		}
		blink_start(&item->blink);
	}
}

void tm1637_clear(void *device){
	TM1637 *item = (TM1637 *) device;
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM1);
	tm1637_stop(item);

	//first digit address
	tm1637_start(item);
	tm1637_write(item, TM1637_I2C_COMM2 + (TM1637_MOD_POS & 0x03));

	//data
	for (uint8_t i=0; i < TM1637_SIGNS_COUNT; i++){
		tm1637_write(item, 0x00);
	}
	tm1637_stop(item);
}


static void tm1637_IDLE(void *device){
	TM1637 *item = (TM1637 *) device;
	Blink *blink = &item->blink;
	CONTROL(blink);
}

static void tm1637_SCROLL(void *device){
	TM1637 *item = (TM1637 *) device;
	if(tonr(&item->tmr)){
		if(item->i >= item->blen + TM1637_SIGNS_COUNT){
			item->control = tm1637_SCROLL_START;
			return;
		}
		for (size_t j = 0; j < TM1637_SIGNS_COUNT - 1; j++) {
			item->signs[j] = item->signs[j + 1];
		}
		uint8_t c = 0;
		if(item->i < item->blen){
			c = item->buf[item->i];
		}
		item->signs[TM1637_SIGNS_COUNT - 1] = c;
		tm1637_setSigns(item, item->signs);
		item->i++;
	}
	Blink *blink = &item->blink;
	CONTROL(blink);
}

static void tm1637_SCROLL_START(void *device){
	TM1637 *item = (TM1637 *) device;
	memset(item->signs, 0, sizeof (*item->signs) * TM1637_SIGNS_COUNT);
	ton_expire(&item->tmr);
	item->i = 0;
	item->control = tm1637_SCROLL;
}

static void tm1637_blinkLow (void *device){
	tm1637_clear(device);
}

static void tm1637_blinkHigh (void *device){
	TM1637 *item = (TM1637 *) device;
	tm1637_setSigns(item, item->signs);
}

void tm1637_control(void *device){
	TM1637 *item = (TM1637 *) device;
	item->control(item);
}

TM1637 *tm1637_new(){
	size_t sz = sizeof (TM1637);
	TM1637 *out = (TM1637 *) malloc(sz);
	if(out == NULL){ printdln("tm1637_new: failed");}
	printd("tm1637_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void tm1637_begin(TM1637 *item, int dio, int clk){
	item->dio = dio;
	item->clk = clk;
	pinMode(item->clk, INPUT);
	pinMode(item->dio,INPUT);
	digitalWrite(item->clk, LOW);
	digitalWrite(item->dio, LOW);
	tm1637_setBrightness(item, TM1637_BRIGHTNESS);
	ton_setInterval(&item->tmr, DISPLAY_SCROLL_INTERVAL_MS);
	ton_reset(&item->tmr);
	blink_begin(&item->blink, item, tm1637_blinkHigh, tm1637_blinkLow);
	item->alignment = DISPLAY_ALIGNMENT_LEFT + DISPLAY_ALIGNMENT_RIGHT;
	item->control = tm1637_IDLE;
}
