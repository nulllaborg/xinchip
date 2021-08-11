#ifndef _BOARD_H_
#define _BOARD_H_
#include "system.h"
#include "xc620.h"

#define ENABLE_TIMER 0
#define ENABLE_GPIO  1
#define ENABLE_I2C   1
#define ENABLE_GPADC 0
#define ENABLE_UART0 1
#define ENABLE_UART1 1

#ifdef ENABLE_GPIO
#include "gpio.h"
#endif

#ifdef ENABLE_GPIO
#include "uart.h"
#endif

#ifdef ENABLE_GPIO
#include "i2c.h"
#endif

void board_init(void);

#endif
