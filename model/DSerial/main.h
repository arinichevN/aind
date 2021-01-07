#ifndef DISPLAY_DSERIAL_H
#define DISPLAY_DSERIAL_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../Display/common.h"
#include "../interface/iDisplay.h"

typedef struct {
	iDisplay im_display;
}DSerial;

extern int dserial_begin(DSerial *self);

#endif
