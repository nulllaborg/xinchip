#ifndef _ADC_H_
#define _ADC_H_

const uint8 adc_Pins[NUM_ANALOG_INPUTS] __FLASH__ = {
    2, 3, 19 , 18 , 0 , 1 , 4 , 5
};

void init_adc(void);
void adc_gpio_config(uint8_t channel);
int get_adc_val(uint8_t ch);
#endif