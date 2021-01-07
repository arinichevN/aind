#ifndef APP_SERIALS_H
#define APP_SERIALS_H


#include "../../lib/acp/loop/client/multi.h"
#include "../../lib/acp/loop/spy/main.h"
#include "../../pmem/main.h"
#include "../AppSerial/main.h"

#define S0 0
#define S1 0
#define S2 0
#define S3 0
#if defined(HAVE_HWSERIAL0) && defined(WANT_SERIAL0)
#define USE_SERIAL0
#undef S0
#define S0 1
#endif
#if defined(HAVE_HWSERIAL1) && defined(WANT_SERIAL1)
#define USE_SERIAL1
#undef S1
#define S1 1
#endif
#if defined(HAVE_HWSERIAL2) && defined(WANT_SERIAL2)
#define USE_SERIAL2
#undef S2
#define S2 1
#endif
#if defined(HAVE_HWSERIAL3) && defined(WANT_SERIAL3)
#define USE_SERIAL3
#undef S3
#define S3 1
#endif
#define SERIAL_COUNT S0 + S1 + S2 + S3

extern AppSerialList serials;

#define APP_SERIALS_IND asrind
#define FOREACH_SERIAL for(size_t APP_SERIALS_IND = 0; APP_SERIALS_IND<serials.length; APP_SERIALS_IND++) {AppSerial *serial = &serials.items[APP_SERIALS_IND];

extern void serials_begin();

extern void serials_control();

extern void serials_free();

extern Acplcm *serials_getClientById(int id);

extern Acply *serials_getSpyById(int id);

extern size_t serials_getIndById(int serial_id);

#endif
