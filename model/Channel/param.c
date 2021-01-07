#include "param.h"

int channelParam_check(const ChannelParam *self){
	if(!common_checkBlockStatus(self->enable)){
		printd("channelParam_check(): bad enable where id = "); printdln(self->id);
		return ERROR_PARAM;
	}
	switch(self->mode){
		case SERIAL_MODE_SERVER:
		case SERIAL_MODE_CLIENT:
		case SERIAL_MODE_SPY:
			break;
		default:
			printd("channelParam_check(): bad mode where id = "); printdln(self->id);
			return ERROR_PARAM;
	}
	switch(self->display_kind){
		case DEVICE_KIND_MAX7219:
		case DEVICE_KIND_TM1637:
		case DEVICE_KIND_DSERIAL:
		case DEVICE_KIND_DSLED:
			break;
		default:
			printd("channelParam_check(): bad display_kind where id = "); printdln(self->id);
			return ERROR_PARAM;
	}
	switch(self->display_text_alignment){
		case DISPLAY_ALIGNMENT_LEFT:
		case DISPLAY_ALIGNMENT_RIGHT:
			break;
		default:
			printd("channelParam_check(): bad display_text_alignment where id = "); printdln(self->id);
			return ERROR_PARAM;
	}
	switch(self->acp_command){
		case CMD_GETR_CHANNEL_FTS:
		case CMD_GETR_CHANNEL_STATE:
		case CMD_GETR_CHANNEL_ERROR:
		case CMD_SET_CHANNEL_GOAL:
			break;
		default:
			printd("channelParam_check(): bad acp_command where id = "); printdln(self->id);
			return ERROR_PARAM;
	}
	switch(self->acp_command){
		case CMD_SET_CHANNEL_GOAL:
		if(self->mode != SERIAL_MODE_SPY){
			printd("channelParam_check(): spy mode expected for set acp command where id = "); printdln(self->id);
			return ERROR_PARAM;
		}
	}
	switch(self->serial_id){
		case SERIAL_ID_NONE:
		case SERIAL_ID_0:
		case SERIAL_ID_1:
		case SERIAL_ID_2:
		case SERIAL_ID_3:
			break;
		default:
			printd("channelParam_check(): bad serial_id where id = "); printdln(self->id);
			return ERROR_PARAM;
	}
	if(self->float_precision < 0 || self->float_precision > CHANNEL_FLOAT_PRECISION_MAX){
		printd("channelParam_check(): bad float_precision where id = "); printdln(self->id);
		return ERROR_PARAM;
	}
    return ERROR_NO;
}
