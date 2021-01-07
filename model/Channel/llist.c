#include "llist.h"

int channelLList_activeExists(ChannelLList *channels){
	FOREACH_LLIST(channel, channels, Channel){
		if(channel->control != channel_OFF){
			return 1;
		}
	}
	return 0;
}

int channelLList_getIdFirst(ChannelLList *channels, int *out){
	int success = 0;
	int f = 0;
	int v;
	FOREACH_LLIST(channel, channels, Channel){
		if(!f) { v=channel->id; f=1; success=1;}
		if(channel->id < v) v = channel->id;
	}
	*out = v;
	return success;
}
