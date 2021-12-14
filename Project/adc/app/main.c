#include <stdio.h>
#include <string.h>
#include "HardwareSerial.h"
#include "Arduino.h"

int main(void)
{
    int val1, val2;
    systick_init();
    Serial1.begin(BAUD_115200);
    init_adc(GADC_FREQ_1M);
	adc_gpio_config(A6);
	adc_config_channel(A6);
    Serial1.printf("adc test demo\r\n");
    while(1) {
        val1 = analogRead(A6);
        // val2 = analogRead(A7);
        Serial1.printf("val1: %d\r\n",val1);
        // Serial1.printf("val2: %d\r\n",val2);
        delay_ms(10);
    };
}
