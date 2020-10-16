#ifndef DISPLAY_DSERIAL_H
#define DISPLAY_DSERIAL_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../common.h"

extern void dserial_clear(void *device);

extern void dserial_printStr(void *device, const char *str, int alignment);

extern void dserial_printBlinkStr(void *device, const char *str, int alignment);

extern void dserial_control(void *device);

#endif
