#include "config.h"

static int appConfig_check (AppConfig *item){
	int r = ERROR_NO;
	FOREACH_SERIAL(i){
		r = appSerialConfig_check(&item->serial[i]);
		if(r != ERROR_NO) return r;
	}
	return r;
}

#define APP_SET_DEFAULT_SERIAL_CONFIG_SPY		item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].mode = SERIAL_MODE_SPY; item->serial[i].id = sid; i++;
#define APP_SET_DEFAULT_SERIAL_CONFIG_CLIENT	item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].mode = SERIAL_MODE_CLIENT; item->serial[i].id = sid; i++;
#define APP_SET_DEFAULT_SERIAL_CONFIG_DEBUG		item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].mode = SERIAL_MODE_DEBUG; item->serial[i].id = sid; i++;
#define APP_SET_DEFAULT_SERIAL_CONFIG_IDLE		item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].mode = SERIAL_MODE_IDLE; item->serial[i].id = sid; i++;

/*
 * -user_config:
 * in this function you can set default configuration for application:
 */
static void appConfig_setDefault(AppConfig *item){
	item->id = DEFAULT_APP_ID;
	size_t i = 0;
	int sid = SERIAL_IDN;
#ifdef USE_SERIAL0
	sid = SERIAL_ID0;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_DEBUG
	
#endif
#ifdef USE_SERIAL1
	sid = SERIAL_ID1;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_CLIENT
	
#endif
#ifdef USE_SERIAL2
	sid = SERIAL_ID2;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_SPY
	
#endif
#ifdef USE_SERIAL3
	sid = SERIAL_ID3;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_SPY
	
#endif
}

int appConfig_begin(AppConfig *item){
	appConfig_setDefault(item);
	printd("set default app\n");
	return appConfig_check(item);
}

APPCONFIG_DEF_GET_FIELD_FUNC(id)
APPCONFIG_DEF_SET_FIELD_FUNC(id)
