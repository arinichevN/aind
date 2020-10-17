#include "main.h"

static void display_setParam(Display *item, int kind, iDisplay *device){
	item->kind = kind;
	item->device = device;
}

void display_free(Display *item){
	item->device->free(item->device->self);
	free(item->device);
}

int display_begin(Display *item, int device_kind, int p1, int p2, int p3){
	item->device = NULL;
	item->kind = DEVICE_KIND_UNKNOWN;
	item->p1 = p1; item->p2 = p2; item->p3 = p3;
	switch(device_kind){
		case DEVICE_KIND_TM1637:
		case DEVICE_KIND_MAX7219:{
				Display7 *device = display7_new();
				if(device == NULL){
					return 0;
				}
				if(!display7_begin(device, device_kind, p1, p2, p3)){
					return 0;
				}
				display_setParam(item, device_kind, &device->im_display);
			}
			return 1;
		case DEVICE_KIND_DSERIAL:{
				DSerial *device = dserial_new();
				if(device == NULL){
					return 0;
				}
				if(!dserial_begin(device)){
					return 0;
				}
				display_setParam(item, device_kind, &device->im_display);
			}
			return 1;
	}
	return 0;
}



