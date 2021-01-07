#include "main.h"

AppSerial serial_buf[SERIAL_COUNT];

AppSerialList serials = {.items = serial_buf, .length = SERIAL_COUNT};

void serials_reset(){
	FOREACH_SERIAL
		appSerial_reset(serial);
	}
}

void serials_setStaticParam(){
	size_t i = 0;
#ifdef USE_SERIAL0
	serials.items[i].device = &Serial;
	serials.items[i].id = SERIAL_ID_0; 
	i++;
#endif
#ifdef USE_SERIAL1
	serials.items[i].device = &Serial1;
	serials.items[i].id = SERIAL_ID_1;
	i++;
#endif
#ifdef USE_SERIAL2
	serials.items[i].device = &Serial2;
	serials.items[i].id = SERIAL_ID_2;
	i++;
#endif
#ifdef USE_SERIAL3
	serials.items[i].device = &Serial3;
	serials.items[i].id = SERIAL_ID_3;
	i++;
#endif
}

#define SET_DEFAULT_SERIAL_PARAM(RATE, CONFIG, MODE) param[i].rate = RATE; param[i].config = CONFIG; param[i].mode = MODE; i++;

int serials_saveDefaultParam(){
	AppSerialParam param[SERIAL_COUNT];
	size_t i = 0;
	int sid = SERIAL_ID_NONE;
#ifdef USE_SERIAL0
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_CONFIG_8N1, SERIAL_MODE_CLIENT)
	
#endif
#ifdef USE_SERIAL1
	
	//-user_config (see ../util/serial.h for available options)
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_CONFIG_8N1, SERIAL_MODE_CLIENT)
	
#endif
#ifdef USE_SERIAL2
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_CONFIG_8N1, SERIAL_MODE_IDLE)
	
#endif
#ifdef USE_SERIAL3
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_CONFIG_8N1, SERIAL_MODE_IDLE)
#endif
	FOREACH_SERIAL
		if(!pmem_saveSerialParam(&param[APP_SERIALS_IND], APP_SERIALS_IND)){
			return 0;
		}
	}
	return 1;
}

int serials_begin(int default_btn){
	serials_reset();
	serials_setStaticParam();
	if(default_btn == BUTTON_DOWN){
		if(!serials_saveDefaultParam()){
			return 0;
		}
		printd("set default app param\n");
	}
	FOREACH_SERIAL
		AppSerialParam param;
		if(!pmem_getSerialParam(&param, APP_SERIALS_IND)){
			return 0;
		}
		if(appSerialParam_check(&param) != ERROR_NO){
			return 0;
		}
		if(appSerial_beginMode(serial, &param, &DEBUG_SERIAL_DEVICE) != ERROR_NO){
			return 0;
		}
	}
	return 1;
}


void serials_control(){
	FOREACH_SERIAL
		CONTROL(serial);
	}
}

void serials_free(){
	FOREACH_SERIAL
		appSerial_free(serial);
	}
}

Acplcm *serials_getClientById(int id){
	FOREACH_SERIAL
		if(id == serial->id && serial->mode == SERIAL_MODE_CLIENT){
			return (Acplcm *) serial->controller;
		}
	}
	return NULL;
}

Acply *serials_getSpyById(int id){
	FOREACH_SERIAL
		if(id == serial->id && serial->mode == SERIAL_MODE_SPY){
			return (Acply *)serial->controller;
		}
	}
	return NULL;
}

size_t serials_getIndById(int serial_id){
	switch(serial_id){
		case SERIAL_ID_0: if(SERIAL_COUNT > 1) return 0; break;
		case SERIAL_ID_1: if(SERIAL_COUNT > 2) return 1; break;
		case SERIAL_ID_2: if(SERIAL_COUNT > 3) return 2; break;
		case SERIAL_ID_3: if(SERIAL_COUNT > 4) return 3; break;
	}
	return -1;
}
