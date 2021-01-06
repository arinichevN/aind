#include "param.h"

int appParam_check (AppParam *self){
	int r = ERROR_NO;
	return r;
}

/*
 * -user_config:
 * in this function you can set default configuration for application:
 */
void appParam_setDefault(AppParam *self){
	self->id = DEFAULT_APP_ID;
}


APP_PARAM_DEF_GET_FIELD_FUNC(id)
APP_PARAM_DEF_SET_FIELD_FUNC(id)
