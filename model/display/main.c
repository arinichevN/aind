#include "main.h"

static void display_setParam(Display *item, void *device, int kind, void (*clear) (void *),	void (*printStr) (void *, const char *, int), void (*printBlinkStr) (void *, const char *, int), void (*control) (void *), void (*free) (void *)){
	item->device = device;
	item->kind = kind;
	item->clear = clear;
	item->printStr = printStr;
	item->printBlinkStr = printBlinkStr;
	item->control = control;
	item->free = free;
}

void display_free(Display *item){
	item->free(item->device);
	free(item->device);
}

int display_begin(Display *item, int device_kind, int p1, int p2, int p3){
	item->printStr = NULL;
	item->printBlinkStr = NULL;
	item->device = NULL;
	item->kind = DEVICE_KIND_UNKNOWN;
	item->p1 = p1; item->p2 = p2; item->p3 = p3;
	switch(device_kind){
		case DEVICE_KIND_TM1637:
		case DEVICE_KIND_MAX7219:{
				DISPLAY7 *device = display7_new();
				if(device == NULL){
					return 0;
				}
				display7_begin(device, device_kind, p1, p2, p3);
				display_setParam(item, device, device_kind, display7_clear, display7_printStr, display7_printBlinkStr, display7_control, display7_free);
			}
			return 1;
		case DEVICE_KIND_DSERIAL:
			display_setParam(item, NULL, device_kind, dserial_clear, dserial_printStr, dserial_printBlinkStr, dserial_control, dserial_free);
			return 1;
	}
	return 0;
}



