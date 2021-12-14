#include "adc.h"
#include "pins_arduino.h"

// bit1基准电压(0：选择2.47V; 1:选择AVDD(3.3V))
void analogReference(uint8_t mode)
{
    adc_set_reference(mode);
}

int analogRead(uint8_t channel)
{
   return get_adc_val(channel);
}

void analogWrite(uint8_t pin, uint8_t val)
{
    //pwm_set(boardPWMPins[pin], 1000, val);
}
