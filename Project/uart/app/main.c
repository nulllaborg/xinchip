#include <stdio.h>
#include <string.h>
#include "board.h"
#include "gpio.h"
#include "HardwareSerial.h"
#include "Arduino.h"

void flash_led(void)
{
    digitalToggle(25);
}

int	main(void)
{
    board_init();
		Serial1.begin(BAUD_115200);
    Serial1.printf("uart test\r\n");
    while(1) {
    }
}
