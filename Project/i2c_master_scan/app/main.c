#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "board.h"
#include "HardwareSerial.h"

int	main(void)
{
    Serial1.begin(BAUD_115200);
    Serial1.printf("i2c scan test\r\n");
	i2c_master_init(GPIO_PIN_5, GPIO_PIN_4, 0x50, I2C_STANDARD_MODE);
    while(1);	
}
