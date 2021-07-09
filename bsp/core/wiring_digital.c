#include "pins_arduino.h"
#include "Arduino.h"
#include "gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint32_t *mod_reg, *pull_reg, *debounce_reg;
    mod_reg = portModeRegister(port);
    debounce_reg = GPIO_DEBOUNCE0 + port;
    if (port > 2) return;
    if (pin < 5) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x38);
    } else if (pin >= 5 && pin < 16) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x34);
    } else if (pin >= 16 && pin <= 28) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x38);
    }

    if (mode == INPUT) {
        *pull_reg &= bitclear(bit<<2);
        *pull_reg &= bitclear((bit<<2)+1);
        *mod_reg &= ~bit;
    } else if (mode == INPUT_PULLUP) {
        *pull_reg |= bit((bit<<2)+1);
        *pull_reg &= bitclear(bit<<2);
        *mod_reg &= ~bit;
    } else if (mode == INPUT_PULLDOWN) {
        *pull_reg |= bit(bit<<2);
        *pull_reg &= bitclear((bit<<2)+1);
        *mod_reg &= ~bit;
    } else if(mode == OUTPUT) {
        *mod_reg |= bit;
    }
    writeReg32(debounce_reg, bit);
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
