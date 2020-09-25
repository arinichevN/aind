void dserial_printStr(void *device, const char *str, int alignment){
	printdln(str);
}

void dserial_printBlinkStr(void *device, const char *str, int alignment){
	printd("BLINK "); printdln(str);
}

void dserial_control(void *device){
	;
}
