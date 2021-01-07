#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "../lib/acp/loop/main.h"
#include "../lib/acp/loop/server/main.h"
#include "../lib/common.h"
#include "../lib/serial.h"
#include "../model/Channel/main.h"
#include "channels/main.h"
#include "serials/main.h"
#include "AppSerial/param.h"
#include "AppErrorIndicator/main.h"
#include "param.h"

extern int app_id;
extern int app_error_id;
extern void (*app_control)();

extern int appc_checkId(int v);

extern void app_begin();
extern void app_reset();

extern const char *app_getErrorStr();
extern const char *app_getStateStr();


#endif 
