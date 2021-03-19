#ifndef iDISPLAY_H
#define iDISPLAY_H

#include "../../lib/common.h"

typedef struct {
	void *self;
	void (*free) (void *);
	void (*clear) (void *);
	void (*printStr) (void *, const char *, talign_t);
	void (*printBlinkStr) (void *, const char *, talign_t);
	void (*control) (void *);
} iDisplay;

#endif
