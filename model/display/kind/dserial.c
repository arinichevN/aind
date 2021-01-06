#include "dserial.h"

void dserial_printStr(void *vself, const char *str, int alignment){
	printdln(str);
}

void dserial_printBlinkStr(void *vself, const char *str, int alignment){
	printd("BLINK "); printdln(str);
}

void dserial_control(void *self){
	;
}

void dserial_free(void *vself){
	printdln("DISPLAY FREE");;
}

void dserial_clear(void *vself){
	printdln("");
}

int dserial_buildInterfaces(DSerial *self){
	self->im_display.self = self;
	self->im_display.clear = dserial_clear;
	self->im_display.control = dserial_control;
	self->im_display.free = dserial_free;
	self->im_display.printStr = dserial_printStr;
	self->im_display.printBlinkStr = dserial_printBlinkStr;
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

int dserial_begin(DSerial *self){
	dserial_buildInterfaces(self);
	return 1;
}
