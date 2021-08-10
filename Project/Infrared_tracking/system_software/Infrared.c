#include "Infrared.h"

void _Infrared_io_init()
{
	
	gpio_mux_ctl(0 , 0);
	gpio_mux_ctl(1 , 0);
	gpio_mux_ctl(19, 0);
	gpio_mux_ctl(20, 0);
	gpio_mux_ctl(21, 0);
	
	gpio_fun_sel(0 , 0);
	gpio_fun_sel(1 , 0);
	gpio_fun_sel(19, 0);
	gpio_fun_sel(20, 0);
	gpio_fun_sel(21, 0);
	
	gpio_fun_inter(0 , 0);
	gpio_fun_inter(1 , 0);
	gpio_fun_inter(19, 0);
	gpio_fun_inter(20, 0);
	gpio_fun_inter(21, 0);
	
	gpio_direction_input(0 , 2);
	gpio_direction_input(1 , 2);
	gpio_direction_input(19, 2);
	gpio_direction_input(20, 2);
	gpio_direction_input(21, 2);
}

uint8_t _DigitalRead(PIN pin)
{
	return gpio_input_val(pin) & 0x01;
}

uint8_t _Infrared_state()
{
	uint8_t state = 0;
	
	state |= (gpio_input_val(pin_1) & 0x01) << 0;
	state |= (gpio_input_val(pin_2) & 0x01) << 1;
	state |= (gpio_input_val(pin_3) & 0x01) << 2;
	state |= (gpio_input_val(pin_4) & 0x01) << 3;
	state |= (gpio_input_val(pin_5) & 0x01) << 4;

	return state;
}

XINC_INFRARED _xinc_infrared = {
	_Infrared_io_init,
	_DigitalRead,
	_Infrared_state,
};