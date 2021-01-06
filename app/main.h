#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "../acp/loop/main.h"
#include "../acp/loop/server/main.h"
#include "../util/common.h"
#include "../util/serial.h"
#include "../model/channel/main.h"
#include "channels.h"
#include "serials.h"
#include "serial_param.h"
#include "param.h"
#include "error_indicator.h"


extern int app_id;
extern int app_error_id;
extern void (*app_control)();


extern int appc_checkId(int v);

extern void app_begin();
extern void app_reset();

extern const char *app_getErrorStr();
extern const char *app_getStateStr();


#endif 
