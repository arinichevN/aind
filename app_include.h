#ifndef APP_INCLUDE_H
#define APP_INCLUDE_H

#include "lib/debug.h"
#include "lib/common.c"
#include "lib/crc.c"
#include "lib/check.c"
#include "lib/ton.c"
#include "lib/serial.c"
#include "lib/caller_queue.c"

#include "lib/acp/main.c"
#include "lib/acp/loop/main.c"
#include "lib/acp/loop/client/main.c"
#include "lib/acp/loop/client/multi.c"
#include "lib/acp/loop/spy/main.c"
#ifdef SERIAL_SERVER
#include "lib/acp/loop/server/main.c"
#endif

#include "pmem/main.c"

#include "model/Blink/main.c"
#include "model/Scroll/main.c"
#include "model/Display/common.c"
#include "model/DSerial/main.c"
#include "model/DSLed/main.c"
#include "model/MAX7219/main.c"
#include "model/TM1637/main.c"
#include "model/Display7/main.c"
#include "model/Display/main.c"

#include "model/Channel/param.c"
#include "model/Channel/main.c"
#include "model/Channel/llist.c"


#ifdef SERIAL_SERVER
#include "app/serials/server/config.c"
#endif
#include "app/AppErrorIndicator/main.c"
#include "app/AppSerial/param.c"
#include "app/AppSerial/main.c"
#include "app/serials/main.c"
#include "app/channels/main.c"
#include "app/param.c"
#include "app/main.c"

#endif 
