#include "main.h"

AppErrorIndicator app_error_indicator;

#define APPEI_ERROR_SHOW digitalWrite(app_error_indicator.pin, HIGH);
#define APPEI_ERROR_HIDE digitalWrite(app_error_indicator.pin, LOW);

static void appei_WAIT_FAILURE(int error_id);

static void appei_WAIT_SUCCESS(int error_id){
	if(error_id == ERROR_NO){
		APPEI_ERROR_HIDE
		app_error_indicator.control = appei_WAIT_FAILURE;
	}
}

static void appei_WAIT_FAILURE(int error_id){
	if(error_id != ERROR_NO){
		printd("app error detected: "); printdln(getErrorStr(error_id));
		APPEI_ERROR_SHOW
		app_error_indicator.control = appei_WAIT_SUCCESS;
	}
}

static void appei_INIT(int error_id){
	if(error_id == ERROR_NO){
		APPEI_ERROR_HIDE
		app_error_indicator.control = appei_WAIT_FAILURE;
	}else{
		APPEI_ERROR_SHOW
		app_error_indicator.control = appei_WAIT_SUCCESS;
	}
}

void appei_begin(int pin){
	app_error_indicator.pin = pin;
	pinMode(app_error_indicator.pin, OUTPUT);
	app_error_indicator.control = appei_INIT;
}

void appei_control(int pin){
	app_error_indicator.control(pin);
}

#undef APPEI_ERROR_SHOW
#undef APPEI_ERROR_HIDE
