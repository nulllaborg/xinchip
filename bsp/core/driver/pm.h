#include "gpio.h"

#define DEEPSLEEP 1

#define GPIO_DEBOUNCE 1 << 3

#define TRIG_HIGH  (GPIO_DEBOUNCE)|0
#define TRIG_LOW   (GPIO_DEBOUNCE)|1
#define TRIG_RISING_EDGE   (GPIO_DEBOUNCE)|2  
#define TRIG_FALLING_EDGE  (GPIO_DEBOUNCE)|3 
#define TRIG_FALL_RIS_EDGE (GPIO_DEBOUNCE)|4

typedef struct {
    uint8_t time;
} ST_RTC_CONFIG;

typedef struct {
    uint8_t enable;
    uint8_t gpio_index;
    uint8_t int_mode;
} ST_RTC_GPIO;

typedef struct {
    ST_RTC_GPIO gpio_resume[5];
    ST_RTC_CONFIG* rtc_resume;
} ST_PM_RESUME_CONFIG;

void pm_goto_sleep(void);
void pm_init_rtc_gpio(uint32_t num, uint8_t inter_mode);
void pm_goto_deepsleep(void);
