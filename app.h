#ifndef APP_H
#define APP_H

/*
 * -user_config:
 * this pin will be high while delay in setup function and in case of 
 * error after setup.
 */
#define INDICATOR_PIN 13

/*
 * -user_config:
 * uncomment serial interfaces you want to use:
 */
#define WANT_SERIAL0
#define WANT_SERIAL1
#define WANT_SERIAL2
#define WANT_SERIAL3

/*
 * -user_config:
 * uncomment the following line if you need debug messages:
 */
//#define MODE_DEBUG

#endif 
