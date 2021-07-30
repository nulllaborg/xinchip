#include "pins_arduino.h"
#include "Arduino.h"
#include "gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
		gpio_fun_sel(pin, GPIO_Dx);
    gpio_mode_config(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    if (val == LOW) {
       gpio_output_low(pin);
    } else {
       gpio_output_high(pin);
    }
}

int digitalRead(uint8_t pin)
{
		return ((int)gpio_input_val(pin));
}

void digitalToggle(uint8_t pin)
{
	  volatile uint32_t *out;
	  uint8_t bitmask = digitalPinToBitMask(pin);
	  uint8_t port = digitalPinToPort(pin);
	  if (port > 2) return;
    out = portOutputRegister(port);
    if((*out) & (0x1UL << bitmask)) {
        digitalWrite(pin, LOW);
    } else {
        digitalWrite(pin, HIGH);
    }
}
