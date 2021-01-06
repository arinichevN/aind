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

#include "model/channel/main.c"
#include "model/channel/llist.c"

#include "acp/main.c"
#include "acp/loop/main.c"

#include "acp/loop/client/main.c"
#include "acp/loop/client/multi.c"
#include "acp/loop/spy/main.c"

#ifdef SERIAL_SERVER
#include "app/server_config.c"
#include "acp/loop/server/main.c"
#endif

#include "app/error_indicator.c"
#include "app/serial_param.c"
#include "app/param.c"
#include "app/serial.c"
#include "app/serials.c"
#include "app/channels.c"
#include "app/main.c"

#endif 
