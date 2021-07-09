#ifndef _ADC_H_
#define _ADC_H_
void init_adc(void);
void adc_gpio_config(uint8_t channel);
int get_adc_val(uint8_t ch);
#endif