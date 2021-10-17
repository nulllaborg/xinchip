#include "pm.h"
 
void pm_goto_sleep(void)
{
    gpio_sleep_config();
    gpio_mux_ctl(GPIO_PIN_0, 0);
    gpio_fun_sel(GPIO_PIN_0, GPIO_Dx);                     //配置睡眠唤醒脚GPIO1为普通GPIO脚
    gpio_mode_config(GPIO_PIN_0, GPIO_INPUT_PULLDOWN);                   //配置睡眠唤醒脚GPIO1输入下拉
    gpio_fun_inter(GPIO_PIN_0, RIS_EDGE_INT);              //配置 GPIO1上升沿中断
    NVIC_EnableIRQ(GPIO_IRQn);                             //使能GPIO中断 

    writeReg32(CPR_SLP_CTL, 0x00);  //OSCEN_CTL =0x0, SYS_SLP_PD_MASK =0x1 关32M时钟，
    writeReg32(CPR_SLPCTL_INT_MASK, 0xFFFFFFF7);//将GPIO中断作为唤醒源，其他中断屏蔽掉
    writeReg32(CPR_SLP_PD_MASK, 0x101);   
    writeReg32(CPR_SLP_SRC_MASK, 0x60006);        //睡眠源 M0 sleep 信号
    *((volatile unsigned *)(CPR_AO_BASE + 0x50)) &= 0xFFFFFFFE; //close rf digital 手动开关RF总开关
    __NOP();
    __NOP();
    __NOP();
    __WFI(); //Wait For Interrupt 等待中断发生(任意中断)
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: pm_init_rtc_gpio
- 函数功能: GPIO当作唤醒源
- 输入参数: gpio编号
- 双边沿中断 0x0c
- 下降沿中断 0x0b
- 上升沿中断 0x0a
- 低电平中断 0x09
- 高电平中断 0x08
----------------------------------------------------------------------------------------------------*/
void pm_init_rtc_gpio(uint32_t num, uint8_t inter_mode)
{
    uint32_t val;
    gpio_mux_ctl(num, 0);
    gpio_fun_sel(num, GPIO_Dx);               //配置睡眠唤醒脚GPIO1为普通GPIO脚
    if (inter_mode == TRIG_LOW || inter_mode == TRIG_RISING_EDGE || inter_mode == TRIG_FALL_RIS_EDGE) {
        gpio_mode_config(num, GPIO_INPUT_PULLUP);             //配置睡眠唤醒脚GPIO1输入上拉
    } else {
        gpio_mode_config(num, GPIO_INPUT_PULLDOWN);             //配置睡眠唤醒脚GPIO1输入下拉
    }
    if (!((inter_mode >= 0x08) &&(inter_mode <= 0x0c))) return;
    writeReg32(CPR_CTLAPBCLKEN_GRCTL, 0x20002);
    writeReg32(CPR_AOCLKEN_GRCTL, 0x20002);

    readReg32(AO_GPIO_MODE, val);
    val &= ~(0x0f << (num << 2));
    val |= (inter_mode << (num << 2));
    writeReg32(AO_GPIO_MODE, val);

    readReg32(AO_GPIO_CTL, val);
    val |= (1 << (8 + num));
    writeReg32(AO_GPIO_CTL, val);
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_Handler(void)
{
    uint32_t stat;
    readReg32(RTC_ISR, stat);
    if (stat != 0) {
        writeReg32(RTC_EOI, stat);
    }
    writeReg32(AO_GPIO_CTL, 0x01 << GPIO_PIN_1);  //清除 RTC GPIO1中断
    writeReg32(AO_GPIO_CTL, 0x100 << GPIO_PIN_1); //使能 RTC GPIO1中断
}

void pm_goto_deepsleep(void)
{
    gpio_sleep_config();
    pm_init_rtc_gpio(GPIO_PIN_0, TRIG_RISING_EDGE);              //使能 RTC GPIO0 上升沿中断
    writeReg32(CPR_SLP_CTL, 0x0);                    //OSCEN_CTL =0x0, SYS_SLP_PD_MASK =0x0 关32M时钟，断电(已经测试过，7.7uA)
    writeReg32(CPR_SLP_PD_MASK, 0x00);
    writeReg32((volatile unsigned *)(0x40002400+0x3c), 0xF);//spi1 flash脚上下拉配置（如果不配置会漏电200多微安）
    writeReg32(CPR_SLPCTL_INT_MASK, 0xFFFFFFEF);// 不屏蔽，无法睡眠-只将RTC中断唤醒解屏蔽
    writeReg32(CPR_SLP_ST, 0x00010001);
    __NOP;__NOP;__NOP;__NOP;__NOP;
    __WFI();
    __NOP;__NOP;__NOP;__NOP;__NOP;
    for(int i = 0; i < 0x445000; i++);
}
