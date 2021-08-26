#ifndef _ADC_H_
#define _ADC_H_

#include <math.h>
#include "board.h"
#include "gpio.h"

static uint8_t adc_Pins[8] = {
    21, 20, 19 , 18 , 0 , 1 , 4 , 5
};

void init_adc(void);
void adc_gpio_config(uint8_t channel);
void adc_config_channel(uint16_t channel);
int get_adc_val(uint8_t ch);
#endif
