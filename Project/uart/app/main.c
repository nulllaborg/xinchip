#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "HardwareSerial.h"
#include "Arduino.h"

int	main(void)
{
    Serial1.begin(BAUD_115200);
    Serial1.printf("uart test\r\n");
    while(1) {
    }
}
