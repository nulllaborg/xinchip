#include "pins_arduino.h"
#include "Arduino.h"
#include "gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    gpio_mode_config(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint32_t *out;

    if (port > 2) return;

    out = portOutputRegister(port);

    if (val == LOW) {
        *out &= ~bit;
    } else {
        *out |= bit;
    }
}

int digitalRead(uint8_t pin)
{
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    if (port == NOT_A_PIN) return LOW;
    if (*portInputRegister(port) & bit) return HIGH;
    return LOW;
}

void digitalToggle(uint8_t pin)
{
    uint8_t port = digitalPinToPort(pin);
    if (port > 2) return;
    if(digitalRead(pin) == LOW) {
        digitalWrite(pin, HIGH);
    } else {
        digitalWrite(pin, LOW);
    }
}
