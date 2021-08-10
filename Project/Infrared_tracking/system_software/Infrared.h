#ifndef 	__INFRARED_H
#define 	__INFRARED_H

#include "bsp_gpio.h"

typedef enum {
	pin_1 = 0x00,
	pin_2 = 0x01,
	pin_3 = 0x13,
	pin_4 = 0x14,
	pin_5 = 0x15,
} PIN;

typedef struct
{
	void (*infrared_io_init)();
	uint8_t (*digitalRead)(PIN pin);
	uint8_t (*infrared_state)();
} XINC_INFRARED;

extern XINC_INFRARED _xinc_infrared;
#endif