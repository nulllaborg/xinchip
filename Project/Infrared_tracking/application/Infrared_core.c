#include "platform.h"

void Gpio_init(XINC_INFRARED_CORE *infrared_core)
{
	Init_gpio();
	infrared_core->xinc_infrared = _xinc_infrared;
	infrared_core->xinc_infrared.infrared_io_init();
}

void DigitalRead(XINC_INFRARED_CORE *infrared_core)
{
	IIC_REC_BUFFER[0x00][0x00] = infrared_core->xinc_infrared.digitalRead(pin_1);
	printf("val1:  %d            \n",IIC_REC_BUFFER[0x00][0x00]);
	IIC_REC_BUFFER[0x01][0x00] = infrared_core->xinc_infrared.digitalRead(pin_2);
	IIC_REC_BUFFER[0x02][0x00] = infrared_core->xinc_infrared.digitalRead(pin_3);
	IIC_REC_BUFFER[0x03][0x00] = infrared_core->xinc_infrared.digitalRead(pin_4);
	IIC_REC_BUFFER[0x04][0x00] = infrared_core->xinc_infrared.digitalRead(pin_5);
	IIC_REC_BUFFER[0x05][0x00] = infrared_core->xinc_infrared.infrared_state();
	printf("val2:  %d            \n",IIC_REC_BUFFER[0x05][0x00]);
}

void AnalogRead(XINC_INFRARED_CORE *infrared_core)
{
	
}				