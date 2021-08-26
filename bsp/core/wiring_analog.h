#ifndef _WIRING_ANALOG_H_
#define _WIRING_ANALOG_H_

#include "board.h"
#include "pins_arduino.h"
#include "Arduino.h"
#include "adc.h"

#if defined(__XC6x0__)
void analogImproving(void);
#endif

void analogReference(uint8_t mode);
int analogRead(uint8_t channel);
void analogWrite(uint8_t pin, uint8_t val);

#endif
