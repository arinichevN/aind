#ifndef DISPLAY_DSERIAL_H
#define DISPLAY_DSERIAL_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "../common.h"

extern void dserial_clear(void *self);

extern void dserial_printStr(void *self, const char *str, int alignment);

extern void dserial_printBlinkStr(void *self, const char *str, int alignment);

extern void dserial_free(void *self);

extern void dserial_control(void *self);

#endif
