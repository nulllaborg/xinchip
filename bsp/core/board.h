#ifndef _BOARD_H_
#define _BOARD_H_
#include "system.h"
#include "xc620.h"

#define ENABLE_TIMER 0
#define ENABLE_GPIO  1
#define ENABLE_I2C   0
#define ENABLE_SPI   0
#define ENABLE_GPADC 0
#define ENABLE_UART0 0
#define ENABLE_UART1 1
#define ENABLE_SPI_FLASH 1

#if ENABLE_GPIO
#include "gpio.h"
#endif

#if ENABLE_GPIO
#include "uart.h"
#endif

#if ENABLE_I2C
#include "i2c.h"
#endif

#if ENABLE_SPI
#include "spi.h"
#endif

#if ENABLE_SPI_FLASH
#include "spi_flash.h"
#endif

void board_init(void);

#endif
