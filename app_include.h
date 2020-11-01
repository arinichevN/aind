#ifndef APP_INCLUDE_H
#define APP_INCLUDE_H

#include "util/debug.h"
#include "util/common.c"
#include "util/crc.c"
#include "util/check.c"
#include "util/ton.c"
#include "util/serial.c"
#include "util/caller_queue.c"

#include "pmem/main.c"

#include "model/display/blink.c"
#include "model/display/scroll.c"
#include "model/display/common.c"
#include "model/display/kind/dserial.c"
#include "model/display/kind/dsled.c"
#include "model/display/kind/display7/MAX7219.c"
#include "model/display/kind/display7/TM1637.c"
#include "model/display/kind/display7/main.c"
#include "model/display/main.c"

#include "model/channel/default.c"
#include "model/channel/main.c"

#include "acp/main.c"
#include "acp/loop/main.c"

#include "acp/loop/client/main.c"
#include "acp/loop/client/multi.c"
#include "acp/loop/spy.c"

#ifdef SERIAL_SERVER
#include "app/server_config.c"
#include "acp/loop/server.c"
#endif

#include "app/error_indicator.c"
#include "app/serial_config.c"
#include "app/config.c"
#include "app/serial.c"
#include "app/main.c"

#endif 
