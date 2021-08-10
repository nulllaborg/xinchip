#include "platform.h"

XINC_INFRARED_CORE infrared_core;

int main()
{	
	Gpio_init(&infrared_core);
	test_uart();
	test_slave_i2c();
	while (1)
	{		
		DigitalRead(&infrared_core);
//		AnalogRead(&infrared_core);
	}
}