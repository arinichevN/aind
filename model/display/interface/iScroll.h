#ifndef DISPLAY_iSCROLL_H
#define DISPLAY_iSCROLL_H

typedef struct {
	void *self;
	void (*printSigns) (void *, const uint8_t *);
} iScroll;


#endif
