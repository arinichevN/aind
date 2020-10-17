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
