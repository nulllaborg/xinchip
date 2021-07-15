#include "wiring_private.h"
#include "pins_arduino.h"

#if defined(__XC6x0__)
void analogImproving(void)
{
    writeReg32(CPR_AO_BASE + 0x20)), 0x2e);
}
#endif

// bit1基准电压(0：选择2.47V; 1:选择AVDD(3.3V))
void analogReference(uint8_t mode)
{
    if (mode) {
        setBitReg32(GPADC_RF_CTL, bit(1));
    } else {
        clearBitReg32(GPADC_RF_CTL, bit(1));
    }
}

int analogRead(uint8_t channel)
{
    get_adc_val(channel);
}

void analogWrite(uint8_t pin, uint8_t val)
{
    pwm_set(boardPWMPins[pin], 1000, val);
}
