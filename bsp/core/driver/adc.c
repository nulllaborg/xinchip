#include "adc.h"
#include "HardwareSerial.h"

uint8_t adc_rx_flag = 0;

void adc_set_reference(uint8_t mode)
{
    if (mode) {
        setBitReg32(GPADC_RF_CTL, (1UL << (1)));
    } else {
        clearBitReg32(GPADC_RF_CTL, (1UL << (1)));
    }
}

void init_adc(void)
{
    clearBitReg32(CPR_RSTCTL_CTLAPB_SW, (1UL << (12)));
    setBitReg32(CPR_RSTCTL_CTLAPB_SW, (1UL << (12)));    /*先使GPADC_RSTN=0，再使 GPADC_RSTN=1，软复位 GPADC 模块*/
    setBitReg32(CPR_CTLAPBCLKEN_GRCTL, 0x20002000);//使能GPADC_PCLK_EN 的GPADC_PCLK时钟
    setBitReg32(GPADC_FIFO_CTL, (1UL << (4)));   //对FIFO进行一次清空操作
    clearBitReg32(GPADC_FIFO_CTL, (1UL << (4)));//
    writeReg32(GPADC_RF_CTL, 0x812);      // 配置寄存器 GPADC_RF_CTL   GPADC_PCLK/(gpadc_clkdiv*2)=16M/16=1M ,select 2.4V vref
    writeReg32(GPADC_MAIN_CTL, 0x09);     // 配置寄存器 GPADC_MAIN_CTL 打开GPADC模块，，数据采集上升沿
}

void adc_gpio_config(uint8_t channel)
{
    gpio_mux_ctl(adc_Pins[channel], 0);
    gpio_fun_sel(adc_Pins[channel], GPIO_Dx);
	gpio_mode_config(adc_Pins[channel], GPIO_INPUT_PULLDOWN);
}

void adc_config_channel(uint16_t channel)
{
    writeReg32(GPADC_CHAN_CTL, channel);
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: get_gpadc_val
- 函数功能: 获取adc通道值
- 输入参数: 要获取的通道号
- 返 回 值：返回adc采样的值     
----------------------------------------------------------------------------------------------------*/
int get_adc_val(uint8_t ch)
{
    uint32_t val;
	uint8_t flag = 0;
    int result = 0;
	while(!flag) {
		readReg32(GPADC_FIFO, val);
		if ((((val >> 26) & 0xF) == ch) && (ch == ((val >> 10) & 0xF))) {
			result = (uint32_t)(val & 0x3FF);
			flag = 1;
		}else if ((((val >> 26) & 0xF) == ch) && (ch != ((val >> 10) & 0xF))) {
			result = (val >> 16) & 0x3FF;
			flag = 1;			
		}else if ((((val >> 26) & 0xF) != ch) && (ch == ((val >> 10) & 0xF))) {
			result = val & 0x3FF;	
			flag = 1;
		}
		setBitReg32(GPADC_FIFO_CTL, (1UL << (4)));
		clearBitReg32(GPADC_FIFO_CTL, (1UL << (4)));
	}
	adc_rx_flag = 0;
    return result;
}
