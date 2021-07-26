#ifndef _BOARD_H_
#define _BOARD_H_
#include "system.h"
#include "xc620.h"

#define ENABLE_TIMER  0
#define ENABLE_GPIO 0
#define ENABLE_GPADC 0
#define ENABLE_UART0 1
#define ENABLE_UART1 1

void board_init(void);

#endif
