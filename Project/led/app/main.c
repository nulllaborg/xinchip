#include <stdio.h>
#include <string.h>
#include "board.h"
#include "gpio.h"

void flash_led(void)
{
    digitalToggle(25);
}

int	main(void)
{
    board_init();
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
	  register_systick_callback(flash_led, 1000);
	  systick_init();
	
    while(1) {
    digitalWrite(24, HIGH);
    delay_us(100000);
    digitalWrite(24, LOW);
    delay_ms(100);
    }
}
