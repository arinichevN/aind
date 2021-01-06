#ifndef APP_CHANNELS_H
#define APP_CHANNELS_H

#include "../util/serial.h"
#include "../model/channel/main.h"
#include "../model/channel/llist.h"
#include "serial.h"

extern ChannelLList channels;

#define FOREACH_CHANNEL FOREACH_LLIST_N(channel, channels, Channel)

extern int channels_activeExists();

extern int channels_getIdFirst(int *out);

extern void channels_begin(int default_btn);

#endif 
