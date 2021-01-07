#include "param.h"

int appSerialParam_check (AppSerialParam *item){
	int r = serial_checkConfig(item->config);
	if(!r){
		return ERROR_SERIAL_CONFIG;
	}
	unsigned long rate = serial_getRate(item->rate);
	if(rate <= 0){
		return ERROR_SERIAL_RATE;
	} 
	if(!serial_checkMode(item->mode)){
		return ERROR_SERIAL_MODE;
	}
	return ERROR_NO;
}

APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(rate)
APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(config)
APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(mode)

APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(rate)
APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(config)
APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(mode)
