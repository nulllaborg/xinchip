#ifndef _ADC_H_
#define _ADC_H_

#include <math.h>
#include "board.h"
#include "gpio.h"

#define FIFO_DEEP   16

#define GADC_FREQ_500K  16
#define GADC_FREQ_1M    8
#define GADC_FREQ_2M    4
#define GADC_FREQ_4M    2
#define GADC_FREQ_8M    1

#define GADC_REF_2_47V   1
#define GADC_REF_AVDD    2


static uint8_t adc_Pins[8] = {
    21, 20, 19 , 18 , 0 , 1 , 4 , 5
};

void init_adc(uint8_t freq);
void adc_set_reference(uint8_t mode);
void adc_gpio_config(uint8_t channel);
void adc_config_channel(uint16_t channel);
int get_adc_val(uint8_t ch);
#endif
