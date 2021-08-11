#include <stdio.h>
#include <string.h>
#include "board.h"

void i2c_callback()
{
	int b = 2;
	i2c_slave_write(b);
}

int	main(void)
{
	init_uart(1, GPIO_PIN_2, GPIO_PIN_3, BAUD_115200);
	i2c_slave_init(GPIO_PIN_5, GPIO_PIN_4, 0x50);
	i2c_slave_register_callback(i2c_callback);
    while(1);	
}
