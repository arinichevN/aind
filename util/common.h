#ifndef COMMON_H
#define COMMON_H

#define STATE_STR_MAX_LENGTH 16
#define ACTIVE_STATE_STR "RUN"
#define UNACTIVE_STATE_STR "OFF"

enum StateE {
	INIT=0,
	OFF,
	FAILURE,
	DISABLE,
	RESET, 
	
	DONE,
	RUN,
	DO,
	IDLE,
	BUSY,
	OUT_MAX,
	OUT_MIN,
	WAIT,
	WAIT_OUT,
	WAIT_HIGH,
	WAIT_LOW,
	WAIT_GAP,
	BLOCK,
	LEVEL_HIGH,
    LEVEL_LOW,
	
	WAIT_IN,
	WAIT_DELAY,
	
	COMMAND,
	LISTEN,
	
	SLAVE_START,
	SLAVE_STOP,
	
	SKIP,
	ON,
	EDIT,
	MOVE,
	READ,
	SAVE,
	RAM,
	NVRAM,
	DSTEP1,
	DSTEP2,
	TEST_UA,
	SPY,
	CLIENT
};

enum ErrorE {
	ERROR_NO = 0,
	ERROR_FIRST = 1,
	ERROR_SOME,
	
	WARNING_READ,
	WARNING_BAD_DATA,
	ERROR_PARAM,
	ERROR_SUBBLOCK,
	ERROR_BLOCK_STATUS,
	ERROR_DEVICE_KIND,
	ERROR_GOAL,
	ERROR_SAVE_GOAL,
	ERROR_METHOD,
	ERROR_OUT,
	ERROR_PID_MODE,
	ERROR_PID_KP,
	ERROR_PID_KI,
	ERROR_PID_KD,
	ERROR_POS2_MODE,
	ERROR_POS2_HYS,
	ERROR_PWM,
	ERROR_PMEM_READ,
	ERROR_READ,
	ERROR_CHANNELS,
	ERROR_SERVER_NODES,
	ERROR_SERIAL,
	ERROR_SERIAL_DEVICE,
	ERROR_SERIAL_RATE,
	ERROR_SERIAL_CONFIG,
	ERROR_SERIAL_KIND,
	ERROR_SERIAL_BEGIN,
	ERROR_NO_SERIAL,
	ERROR_SEND,
	ERROR_BAD_RESULT,
	ERROR_PATIENCE_LOST,
	ERROR_SLAVE_START,
	ERROR_SLAVE_STOP,
	ERROR_RETRY,
	ERROR_TERMOCOUPLE_OPEN,
	ERROR_TERMOCOUPLE_SC_VCC,
	ERROR_TERMOCOUPLE_SC_GND,
	ERROR_RTC,
	ERROR_COUNT
};

#define BUTTON_DOWN 0
#define BUTTON_UP 1

#define ERROR_NO 0
typedef enum {
	NO=0,
	YES=1
} EnableE;

typedef enum {
	DEVICE_KIND_UNKNOWN=0,
	DEVICE_KIND_DSERIAL,
	DEVICE_KIND_MAX6675,
	DEVICE_KIND_MAX31855,
	DEVICE_KIND_DS18B20,
	DEVICE_KIND_SPWM,
	DEVICE_KIND_HPWM,
	DEVICE_KIND_DHT22,
	DEVICE_KIND_DHT22T,	//temperature
	DEVICE_KIND_DHT22H,	//humidity
	DEVICE_KIND_DS3231,
	DEVICE_KIND_TIMER,
	DEVICE_KIND_INDICATOR,
	DEVICE_KIND_MAX7219,
	DEVICE_KIND_TM1637
} DeviceKind;

struct timespec{
	unsigned long tv_sec;
	unsigned long tv_nsec;
};

typedef struct {
	double value;
	struct timespec tm;
	int state;
} FTS;

#define ARRLEN(A) (sizeof(A) / sizeof((A)[0]))

#define PERCENT_MAX 100

#define MIN_IND 0UL

#define FLOAT_SURROGATE_FORMAT "%d.%.3d"
#define FLOAT_SURROGATE_DEF(W, F, V) int W = (int)V;	int F = (int)((V - (double)W)*1000);

extern void snprinttime(unsigned long v, char *buf, size_t len);

extern long int getSerialRate (uint8_t kind);

extern uint8_t getSerialRateKind (long int v);

extern long int getSerialConfig(uint8_t kind);

extern uint8_t getSerialConfigKind(long int v);

extern double tsToDouble(struct timespec v);

extern const char *getErrorStr(int v);

#endif 
