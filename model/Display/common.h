#ifndef DISPLAY_COMMON_H
#define DISPLAY_COMMON_H

#include <string.h>
#include <inttypes.h>

#define DISPLAY_BUF_LEN					16

#define DISPLAY_FIRST_CHAR		' '
#define DISPLAY_LAST_CHAR		'z'

enum {
	DISPLAY_MODE_UNKNOWN = 0,
	DISPLAY_MODE_LIGHT = 1,
	DISPLAY_MODE_BLINK
};

extern uint8_t encodeASCII(uint8_t chr);

extern uint8_t encodeASCIISwap(int chr);

extern size_t dstrlen(const char *str);

extern uint8_t encodeSegmentFromStrSwap(const char *str, size_t slen, size_t *ind);

#endif
