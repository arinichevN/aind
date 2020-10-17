#include "dserial.h"

void dserial_printStr(void *self, const char *str, int alignment){
	printdln(str);
}

void dserial_printBlinkStr(void *self, const char *str, int alignment){
	printd("BLINK "); printdln(str);
}

void dserial_control(void *self){
	;
}

void dserial_free(void *self){
	printdln("DISPLAY FREE");;
}

void dserial_clear(void *self){
	printdln("");
}

int dserial_buildInterfaces(DSerial *item){
	item->im_display.self = item;
	item->im_display.clear = dserial_clear;
	item->im_display.control = dserial_control;
	item->im_display.free = dserial_free;
	item->im_display.printStr = dserial_printStr;
	item->im_display.printBlinkStr = dserial_printBlinkStr;
	return 1;
}

DSerial *dserial_new(){
	size_t sz = sizeof (DSerial);
	DSerial *out = (DSerial *) malloc(sz);
	if(out == NULL){ printdln("dserial_new: failed");}
	memset(out, 0, sz);
	printd("dserial_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int dserial_begin(DSerial *item){
	dserial_buildInterfaces(item);
	return 1;
}
