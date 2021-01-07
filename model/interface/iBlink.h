#ifndef DISPLAY_iBLINK_H
#define DISPLAY_iBLINK_H

typedef struct {
	void *self;
	void (*high)(void *);
	void (*low)(void *);
} iBlink;

#endif
