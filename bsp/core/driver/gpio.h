#ifndef _GPIO_H_
#define _GPIO_H_

#include "board.h"

#define DETECT_INTER_NUM(num,val)  ((val>>num)&0x01)

typedef enum{
    NOT_INT = 0,
    NA1_INT,
    NA2_INT,
    NA3_INT,
    NA4_INT,
    RIS_EDGE_INT,//上升沿中断
    NA6_INT,
    FAIL_EDGE_INT,//下降沿中断
    NA8_INT,
    RIS_FAIL_EDGE_INT,//上升沿下降沿中断
}GPIO_INT_TypeDef;

typedef enum{
    GPIO_PIN_0=0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_16,
    GPIO_PIN_17,
    GPIO_PIN_18,
    GPIO_PIN_19,
    GPIO_PIN_20,
    GPIO_PIN_21,
    GPIO_PIN_22,
    GPIO_PIN_23,
    GPIO_PIN_24,
    GPIO_PIN_25,
    GPIO_PIN_26,
    GPIO_PIN_27,
    GPIO_PIN_28,
    GPIO_PIN_29,
    GPIO_PIN_30,
    GPIO_PIN_31,
}GPIO_Pin_TypeDef;

typedef enum{
    GPIO_INPUT = 0, /*上拉输入*/
    GPIO_INPUT_PULLUP,   /*下拉输入*/
    GPIO_INPUT_PULLDOWN,     /*浮空输入*/
	  GPIO_OUTPUT
}GPIO_InputMode_TypeDef;

/* ---------------------------------------------------------------------------------------------------
-  IO复用控制 -
----------------------------------------------------------------------------------------------------*/
typedef union gpio_fun_config {
    uint32_t config;
    struct bit {
        uint8_t b0004:5;
        uint8_t b0812:5;
        uint8_t b1620:5;
        uint8_t b2428:5;
    }bits;
}gpio_fun_config_t;

typedef struct gpio_fun_sel_config{
    gpio_fun_config_t fun_sel0;
    gpio_fun_config_t fun_sel1;
    gpio_fun_config_t fun_sel2;
    gpio_fun_config_t fun_sel3;
    gpio_fun_config_t fun_sel4;
    gpio_fun_config_t fun_sel5;
    gpio_fun_config_t fun_sel6;
    gpio_fun_config_t fun_sel7;
}gpio_fun_sel_config_t;


/* ---------------------------------------------------------------------------------------------------
- 	IO功能
----------------------------------------------------------------------------------------------------*/
typedef union mux1_config {
    uint32_t muxctl1;
    struct bit1 {
        uint8_t gpio0:2;
        uint8_t gpio1:2;
        uint8_t gpio2:2;
        uint8_t gpio3:2;
        uint8_t gpio4:2;
        uint8_t gpio5:2;
        uint8_t gpio6:2;
        uint8_t gpio7:2;
        uint8_t gpio8:2;
        uint8_t gpio9:2;
        uint8_t gpio10:2;
        uint8_t gpio11:2;
        uint8_t gpio12:2;
        uint8_t gpio13:2;
        uint8_t gpio14:2;
        uint8_t gpio15:2;
    } pad1;
}mux1_config_t;

typedef union mux2_config {
    uint32_t muxctl2;
    struct bit2 {
        uint8_t gpio16:2;
        uint8_t gpio17:2;
        uint8_t gpio18:2;
        uint8_t gpio19:2;
        uint8_t gpio20:2;
        uint8_t gpio21:2;
        uint8_t gpio22:2;
        uint8_t gpio23:2;
        uint8_t gpio24:2;
        uint8_t gpio25:2;
        uint8_t gpio26:2;
        uint8_t gpio27:2;
        uint8_t gpio28:2;
        uint8_t bootctl:2;
        uint16_t swk:2;
        uint8_t swd:2;
    }pad2;
}mux2_config_t;

typedef struct gpio_config {
    mux1_config_t mux1_ctl;
    mux2_config_t mux2_ctl;
}gpio_config_t;

/*---------------------------------------------------------------------------------------------------
- IO中断模式
----------------------------------------------------------------------------------------------------*/
typedef union int_config {
    uint32_t interrupt;
    struct bitt {
        uint8_t mode0:4;
        uint8_t mode1:4;
        uint8_t mode2:4;
        uint8_t mode3:4;
        uint16_t reserved;
    }pad;
}int_config_t;

typedef struct int_config_i {
    int_config_t intr_ctl0;
    int_config_t intr_ctl1;
    int_config_t intr_ctl2;
    int_config_t intr_ctl3;
    int_config_t intr_ctl4;
    int_config_t intr_ctl5;
    int_config_t intr_ctl6;
    int_config_t intr_ctl7;
}interrupt_config_t;


void init_gpio(void);
void gpio_mux_ctl(uint8_t num, uint8_t mux);
void gpio_fun_inter(uint8_t num, uint8_t inter);
void gpio_fun_sel(uint8_t num, uint8_t sel);
void gpio_mode_config(uint8_t pin, uint8_t type);
void gpio_output_high(uint8_t num);
void gpio_output_low(uint8_t num);
#endif
