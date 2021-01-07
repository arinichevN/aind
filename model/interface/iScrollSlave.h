#ifndef DISPLAY_iSCROLL_SLAVE_H
#define DISPLAY_iSCROLL_SLAVE_H

typedef struct {
	void *self;
	void (*printSigns) (void *, const uint8_t *);
} iScrollSlave;


#endif
