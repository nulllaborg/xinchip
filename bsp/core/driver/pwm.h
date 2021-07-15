#ifndef _PWM_H_
#define _PWM_H_

#define PWM_CLOCK  160000

void init_pwm(void);
void pwm_set(uint8_t ch, uint16_t freq, uint8_t duty_cycle);
void pwm_gpio_config(uint8_t pwm_ch);
void pwm0_gpio_config(uint8_t gpio_index);
void pwm0inv_gpio_config(uint8_t gpio_index);
void pwm1_gpio_config(uint8_t gpio_index);
void pwm1inv_gpio_config(uint8_t gpio_index);

#endif