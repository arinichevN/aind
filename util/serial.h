#ifndef SERIAL1_H
#define SERIAL1_H

enum SerialIdE{
	SERIAL_ID_NONE,
	SERIAL_ID_0,
	SERIAL_ID_1,
	SERIAL_ID_2,
	SERIAL_ID_3
};

enum SerialRateE{
	SERIAL_RATE_NONE,
	SERIAL_RATE_300,
	SERIAL_RATE_1200,
	SERIAL_RATE_2400,
	SERIAL_RATE_4800, 
	SERIAL_RATE_9600,
	SERIAL_RATE_14400,
	SERIAL_RATE_19200,
	SERIAL_RATE_28800,
	SERIAL_RATE_38400,
	SERIAL_RATE_57600, 
	SERIAL_RATE_115200
};

enum SerialConfigE{
	SERIAL_CONFIG_NONE,
	SERIAL_CONFIG_5N1,
	SERIAL_CONFIG_6N1,
	SERIAL_CONFIG_7N1,
	SERIAL_CONFIG_8N1,
	SERIAL_CONFIG_5N2,
	SERIAL_CONFIG_6N2,
	SERIAL_CONFIG_7N2,
	SERIAL_CONFIG_8N2,
	SERIAL_CONFIG_5E1,
	SERIAL_CONFIG_6E1,
	SERIAL_CONFIG_7E1,
	SERIAL_CONFIG_8E1,
	SERIAL_CONFIG_5E2,
	SERIAL_CONFIG_6E2,
	SERIAL_CONFIG_7E2,
	SERIAL_CONFIG_8E2,
	SERIAL_CONFIG_5O1,
	SERIAL_CONFIG_6O1,
	SERIAL_CONFIG_7O1,
	SERIAL_CONFIG_8O1,
	SERIAL_CONFIG_5O2,
	SERIAL_CONFIG_6O2,
	SERIAL_CONFIG_7O2,
	SERIAL_CONFIG_8O2
};

enum SerialKindE{
	SERIAL_MODE_IDLE,
	SERIAL_MODE_SERVER,
	SERIAL_MODE_CLIENT,
	SERIAL_MODE_DEBUG,
	SERIAL_MODE_SPY
};

extern long int serial_getRate (int v);
extern int serial_getRateKind (long int v);
extern int serial_checkConfig(int v);
extern int serial_checkMode(int v);
extern int serial_checkRate(int v);
extern int serial_getArduinoConfig(int v);
extern const char *serial_getConfigStr(int v);
extern int serial_checkId(int id);
extern HardwareSerial *serial_getDeviceById(int id);
extern const char *serial_getIdStr(int id);
extern const char *serial_getModeStr(int v);

#endif 
