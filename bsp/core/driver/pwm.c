#include "board.h"
#include "gpio.h"

uint8 pwm_pins[]  = {
    0xFF, 0xFF, 0 , 1 , 12 , 13
};

/* ---------------------------------------------------------------------------------------------------
- 函数名称: pwm_set
- 函数功能: pwm初始化
- 输入参数: pwm口0或1,占空比设置（范围0--99），周期计数器设置影响pwm信号输出频率(输出频率=pwm_clk/((period + 1)*100))
----------------------------------------------------------------------------------------------------*/
void pwm_set(uint8_t ch, uint16_t freq, uint8_t duty_cycle)
{
    if (ch > 5) return;
    writeReg32(PWMx_EN(ch), 0x0);//暂停相应的 PWM 周期计数器和占空比计数器 
    writeReg32(PWMx_P(ch), (PWM_PCLK/100/freq - 1)); //pwm周期设置寄存器--等于freq = pwm_clk/((period + 1)*100)
    writeReg32(PWMx_OCPY(ch), duty_cycle);//pwm占空比设置寄存器
    writeReg32(PWMx_UP(ch), 0x1);//更新周期寄存器和占空比寄存器。
    writeReg32(PWMx_EN(ch), 0x1);//使能相应的PWM周期计数器和占空比计数器
}

void init_pwm(void)
{
    CTL_APBPclkCmd(PWM_PCLK, TRUE); //PWM_PCLK 时钟使能
    writeReg32(CPR_PWM_CLK_CTL, (160000/PWM_CLOCK-1));
     /*
    writeReg32(CPR_PWM_CLK_CTL,0x80000000); //pwm_clk=mclk_in(32M)/(2*(0 +1))= 16M ; max output HZ 160KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x80000001); //pwm_clk=mclk_in(32M)/(2*(1 +1))=  8M ; max output HZ  80KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x80000003); //pwm_clk=mclk_in(32M)/(2*(3 +1))=  4M ; max output HZ  40KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x80000007); //pwm_clk=mclk_in(32M)/(2*(7 +1))=  2M ; max output HZ  20KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x80000009); //pwm_clk=mclk_in(32M)/(2*(9 +1))=1.6M ; max output HZ  16KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x8000000F); //pwm_clk=mclk_in(32M)/(2*(15+1))=  1M ; max output HZ  10KHZ
    writeReg32(CPR_PWM_CLK_CTL,0x8000004F); //pwm_clk=mclk_in(32M)/(2*(79+1))=0.2M ; max output HZ   2KHZ
    */
}

// ch: 2-5
void pwm_gpio_config(uint8_t pwm_ch)
{
    if (ch == 0 || ch == 1) return;
    if (ch == 2 || ch == 3) {
        gpio_mux_ctl(pwm_pins[pwm_ch], 2);
    } else if (ch == 4 || ch == 5) {
        gpio_mux_ctl(pwm_pins[pwm_ch], 3);
    }
}

void pwm0_gpio_config(uint8_t gpio_index)
{
    if ((gpio_index >= 11) && (gpio_index <= 13))
    {
        gpio_mux_ctl(gpio_index, 1);
    } else {
        gpio_mux_ctl(gpio_index, 0);
    }
    gpio_fun_sel(gpio_index, PWM0);
}

void pwm0inv_gpio_config(uint8_t gpio_index)
{
    if ((gpio_index >= 11) && (gpio_index <= 13))
    {
        gpio_mux_ctl(gpio_index, 1);
    } else {
        gpio_mux_ctl(gpio_index, 0);
    }
    gpio_fun_sel(gpio_index, PWM0_INV);
}

void pwm1_gpio_config(uint8_t gpio_index)
{
    if ((gpio_index >= 11) && (gpio_index <= 13))
    {
        gpio_mux_ctl(gpio_index, 1);
    } else {
        gpio_mux_ctl(gpio_index, 0);
    }
    gpio_fun_sel(gpio_index, PWM1);
}

void pwm1inv_gpio_config(uint8_t gpio_index)
{
    if ((gpio_index >= 11) && (gpio_index <= 13))
    {
        gpio_mux_ctl(gpio_index, 1);
    } else {
        gpio_mux_ctl(gpio_index, 0);
    }
    gpio_fun_sel(gpio_index, PWM1_INV);
}
