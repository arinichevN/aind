#ifndef MODEL_CHANNEL_LLIST_H
#define MODEL_CHANNEL_LLIST_H

#include "main.h"

DEC_LLIST(Channel)

extern int channelLList_activeExists(ChannelLList *channels);

extern int channelLList_getIdFirst(ChannelLList *channels, int *out);

#endif 
