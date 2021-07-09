#include "adc.h"

void init_adc(void)
{
    setClearReg32(CPR_RSTCTL_CTLAPB_SW, bit(12));
    setBitReg32(CPR_RSTCTL_CTLAPB_SW, bit(12));    /*先使GPADC_RSTN=0，再使 GPADC_RSTN=1，软复位 GPADC 模块*/
    setBitReg32(CPR_CTLAPBCLKEN_GRCTL, 0x20002000);//使能GPADC_PCLK_EN 的GPADC_PCLK时钟
    setBitReg32(GPADC_FIFO_CTL, bit(5));//对 FIFO 进 行 一 次 清 空 操 作
    setClearReg32(GPADC_FIFO_CTL, bit(5));//
    writeReg32(GPADC_RF_CTL, 0x810);// 配置寄存器 GPADC_RF_CTL   GPADC_PCLK/(gpadc_clkdiv*2)=16M/16=1M ,select 2.4V vref
    writeReg32(GPADC_MAIN_CTL, 0x09);// 配置寄存器 GPADC_MAIN_CTL 打开GPADC模块，，数据采集上升沿
}

void adc_gpio_config(uint16_t pin)
{
    gpio_mux_ctl(pin, 0);
    gpio_fun_sel(pin, GPIO_Dx);
    gpio_fun_inter(pin, NO_INT);
}

void adc_config_channel(uint16_t channel)
{
    writeReg32(GPADC_CHAN_CTL, channel);
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: get_gpadc_val
- 函数功能: 获取adc通道值
- 输入参数: 要获取的通道号
- 返 回 值：返回adc采样的值 获取失败返回负值
- 创建日期: 2019-05-23
- 注    意：通道8 --测量内部VDD电压(3.3*(2/3)=2.2V --测量外部满量程为1023 2.4v        
----------------------------------------------------------------------------------------------------*/
int get_adc_val(uint8_t ch)
{
    uint32_t val;
    int result = 0;
    uint8_t count=5;
    while (count--)
    {
        readReg32(GPADC_FIFO, val);
        if ((((val >> 26) & 0xF) == ch) && (ch == ((val >> 10)&0xF))) {
            
            if (abs(((val >> 16) & 0x03FF)-(val&0x3FF)) > 25)
            {
                result = -2;
            } else {
                result = ((uint32_t)(val&0x3FF) + (uint32_t)(((val>>16)&0x03FF)))/2;
                break;
            }   
        } else {
            result = -1;
        }
    }
    writeReg32(GPADC_FIFO_CTL, 0x10); //对FIFO进行一次清空操作
	writeReg32(GPADC_FIFO_CTL, 0x00); //注意读一次后一定要清空FIFO否则会引起严重的滞后效应
    return result;
}



