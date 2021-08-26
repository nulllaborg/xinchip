#include <stdio.h>
#include <string.h>
#include "HardwareSerial.h"
#include "wiring_analog.h"

int main(void)
{
    int val1,val2;
    init_adc();
    Serial1.begin(BAUD_115200);
    while(1){
        val1 = analogRead(6);
        val2 = analogRead(7);
        Serial1.printf("val1: %d\r\n",val1);
        Serial1.printf("val2: %d\r\n",val2);
    };
}
