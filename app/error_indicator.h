#ifndef APP_ERROR_INDICATOR_H
#define APP_ERROR_INDICATOR_H

typedef struct apperrorindicator_st AppErrorIndicator;
struct apperrorindicator_st{
	int pin;
	void (*control)(int);
};

extern AppErrorIndicator app_error_indicator;

extern void appei_begin(int pin);

extern void appei_control(int pin);

#endif
