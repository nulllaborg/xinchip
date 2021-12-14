#include <stdio.h>
#include <string.h>
#include "board.h"
#include "Arduino.h"

void flash_led_24(void)
{
    digitalToggle(24);
}

void flash_led(void)
{
    digitalToggle(25);
}

int main(void)
{
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    Timer0Set(500000, flash_led_24);
    Timer0Start();
    Timer1Set(1000000, flash_led);
    Timer1Start();
    while(1);
}
