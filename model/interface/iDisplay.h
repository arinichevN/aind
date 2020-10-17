#ifndef iDISPLAY_H
#define iDISPLAY_H

typedef struct {
	void *self;
	void (*free) (void *);
	void (*clear) (void *);
	void (*printStr) (void *, const char *, int);
	void (*printBlinkStr) (void *, const char *, int);
	void (*control) (void *);
} iDisplay;

#endif
