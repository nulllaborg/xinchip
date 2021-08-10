#ifndef 	__INFRARED_CORE_H
#define 	__INFRARED_CORE_H

#include "Infrared.h"

typedef enum {
	REG_Digital = 0x00,
	REG_Analog = 0x10,
	INFRARED_REG_MAX = 0x10,
} INFRARED_REG;

typedef struct 
{
	XINC_INFRARED xinc_infrared;
} XINC_INFRARED_CORE;

extern void Gpio_init(XINC_INFRARED_CORE *infrared_core);
extern void DigitalRead(XINC_INFRARED_CORE *infrared_core);
extern void AnalogRead(XINC_INFRARED_CORE *infrared_core);
#endif