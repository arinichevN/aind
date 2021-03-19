#include "main.h"

static void display_setParam(Display *self, dk_t kind, iDisplay *device){
	self->kind = kind;
	self->device = device;
}

void display_free(Display *self){
	self->device->free(self->device->self);
	free(self->device);
}

int display_begin(Display *self, dk_t device_kind, int p1, int p2, int p3){
	self->device = NULL;
	self->kind = DEVICE_KIND_UNKNOWN;
	self->p1 = p1; self->p2 = p2; self->p3 = p3;
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
				display_setParam(self, device_kind, &device->im_display);
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
				display_setParam(self, device_kind, &device->im_display);
			}
			return 1;
		case DEVICE_KIND_DSLED:{
				DSLed *device = dsled_new();
				if(device == NULL){
					return 0;
				}
				if(!dsled_begin(device, p1, p2, p3)){
					return 0;
				}
				display_setParam(self, device_kind, &device->im_display);
			}
			return 1;
		default:
			break;
	}
	return 0;
}



