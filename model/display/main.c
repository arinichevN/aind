#include "main.h"

static void display_setParam(Display *item, void *device, int kind, void (*clear) (void *),	void (*printStr) (void *, const char *, int), void (*printBlinkStr) (void *, const char *, int), void (*control) (void *)){
	item->device = device;
	item->kind = kind;
	item->clear = clear;
	item->printStr = printStr;
	item->printBlinkStr = printBlinkStr;
	item->control = control;
}

void display_free(Display *item){
	free(item->device);
}

int display_begin(Display *item, int device_kind, int p1, int p2, int p3){
	item->printStr = NULL;
	item->printBlinkStr = NULL;
	item->device = NULL;
	item->kind = DEVICE_KIND_UNKNOWN;
	item->p1 = p1; item->p2 = p2; item->p3 = p3;
	switch(device_kind){
		case DEVICE_KIND_TM1637:{
				TM1637 *device = tm1637_new();
				if(device == NULL){
					return 0;
				}
				tm1637_begin(device, p1, p2);
				display_setParam(item, device, device_kind, tm1637_clear, tm1637_printStr, tm1637_printBlinkStr, tm1637_control);
			}
			return 1;
		case DEVICE_KIND_MAX7219:{
				MAX7219 *device = max7219_new();
				if(device == NULL){
					return 0;
				}
				max7219_begin(device, p1, p2, p3);
				display_setParam(item, device, device_kind, max7219_clear, max7219_printStr, max7219_printBlinkStr, max7219_control);
			}
			return 1;
		case DEVICE_KIND_DSERIAL:
			display_setParam(item, NULL, device_kind, dserial_clear, dserial_printStr, dserial_printBlinkStr, dserial_control);
			return 1;
	}
	return 0;
}



