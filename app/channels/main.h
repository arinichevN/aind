#ifndef APP_CHANNELS_H
#define APP_CHANNELS_H

#include "../../lib/dstructure.h"
#include "../../lib/serial.h"
#include "../../lib/common.h"
#include "../../lib/acp/command/main.h"
#include "../../model/Display/common.h"
#include "../../model/Channel/main.h"
#include "../../model/Channel/llist.h"
#include "../AppSerial/main.h"

extern ChannelLList channels;

#define FOREACH_CHANNEL FOREACH_LLIST_N(channel, channels, Channel)

extern int channels_activeExists();

extern int channels_getIdFirst(int *out);

extern void channels_begin(int default_btn);

#endif 
