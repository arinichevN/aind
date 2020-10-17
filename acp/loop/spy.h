#ifndef ACPL_SPY_H
#define ACPL_SPY_H

#include "main.h"

typedef struct acply_client_st{
	void *self;
	int (*onRequestFunction) (void *, char *, int, int);
	void (*onResponseFunction)(void *, char *, int, int);
} iACPLYClient;

typedef struct {
	iACPLYClient **items;
	size_t length;
}ACPLYClientList;

typedef struct acply_st{
	int last_id;
	ACPLYClientList client_list;
	ACPL *acpl;
	void (*control) (struct acply_st *, HardwareSerial *);
} ACPLY;

extern int acply_initClients(ACPLY *item, size_t count);

extern int acply_addClient(ACPLY *item, iACPLYClient *client);

extern int acply_delClient(ACPLY *item, iACPLYClient *client);

extern void acply_free(ACPLY *item);

extern int acply_begin(ACPLY **item);

extern int acply_setParam(ACPLY *item, size_t client_count);

#define acply_control(item, serial) (item)->control(item, serial)

#endif
