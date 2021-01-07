#ifndef DISPLAY_i7SEGMENT_H
#define DISPLAY_i7SEGMENT_H

typedef struct {
	void *self;
	void (*clear)(void *);
	void (*printSigns)(void *, const uint8_t *);
} i7Segment;

#endif
