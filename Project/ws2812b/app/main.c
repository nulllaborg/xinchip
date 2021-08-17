#include <stdio.h>
#include <string.h>
#include "board.h"
#include "gpio.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "ws2812b.h"

void flash_led(void)
{
    digitalToggle(25);
}

int	main(void)
{
    board_init();
	  systick_init();
	  pinMode(25, OUTPUT);
		Serial1.begin(BAUD_115200);
	  delay_ms(200);
	  Serial1.printf("ws2812b test demo\r\n");
	  ws2812b_init();
	  ws2812b_reset();
	  set_pixel_color(Blue);

    while(1) {
			digitalToggle(25);
      //digitalToggle(22);
			delay_ms(500);
    }
}
