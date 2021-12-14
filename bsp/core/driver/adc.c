#include "adc.h"
#include "spi_flash.h"
#include "HardwareSerial.h"

uint32_t calibration_param = 0;
#if defined(__XC6x0__)
void analogImproving(void)
{
    writeReg32(CPR_AO_BASE + 0x20)), 0x2e);
}
#endif

#define PROG_SECTOR_NUM 60 // = 4*60 = 240k
#define CALI_CENTER_VL  10000
#define CALI_OFFSET_VL  1000

/*
1、在芯片FT测试过程中，将ADC工艺误差校正。
2、ADC校准值(数据)存储于FLASH第240K位置，占用8Byte.(4+4Byte 校准数据和取反校验)
3、校准中心值CALI_CENTER_VL=10000，ADC计算时，将ADC原始值乘以校准值再除以中心值(10000)
  value_calibrated = value * calibration_param / CALI_CENTER_VL ;
4、读取校准值接口：int gadc_calibration_get(uint32_t *value)
   具体用法请参考adc-demo，调用接口返回值大于0表示正确获取到校准值
*/
static int adc_get_calibration(uint32_t *value)
{
    uint32_t vl = 0;
    uint32_t neg = 0;
    uint8_t get_buf[12] = {0};
    init_spi_flash(0, SPIM_CLK_16MHZ);
    spi_flash_release_powerdown();
    spi_flash_read(PROG_SECTOR_NUM*FLASH_SECTOR_SIZE, get_buf, 8);

    spi_flash_enter_powerdown();

    vl  = get_buf[0]*0x1000000 + get_buf[1]*0x10000 + get_buf[2]*0x100 + get_buf[3];
    neg = get_buf[4]*0x1000000 + get_buf[5]*0x10000 + get_buf[6]*0x100 + get_buf[7];
    if((neg == ~vl) && ( (vl > CALI_CENTER_VL-CALI_OFFSET_VL)&&(vl < CALI_CENTER_VL+CALI_OFFSET_VL) ))
    {
        *value = vl;
        return 1;
    } else {
        return -1;
    }
}

void adc_set_reference(uint8_t mode)
{
    if (mode) {
        setBitReg32(GPADC_RF_CTL, (1UL << (1)));
    } else {
        clearBitReg32(GPADC_RF_CTL, (1UL << (1)));
    }
}

void init_adc(uint8_t freq)
{
    clearBitReg32(CPR_RSTCTL_CTLAPB_SW, (1UL << (12)));
    setBitReg32(CPR_RSTCTL_CTLAPB_SW, (1UL << (12)));    /*先使GPADC_RSTN=0，再使 GPADC_RSTN=1，软复位 GPADC 模块*/
    setBitReg32(CPR_CTLAPBCLKEN_GRCTL, 0x20002000);//使能GPADC_PCLK_EN 的GPADC_PCLK时钟

    setBitReg32(GPADC_FIFO_CTL, (1UL << (4)));   //对FIFO进行一次清空操作
    clearBitReg32(GPADC_FIFO_CTL, (1UL << (4))); //中断阈值
	if ((freq>GADC_FREQ_500K) || (freq<GADC_FREQ_8M)) freq = GADC_FREQ_1M;
    
    writeReg32(GPADC_RF_CTL, (freq<<8)|0x12);      // 配置寄存器 GPADC_RF_CTL  GPADC_PCLK/(gpadc_clkdiv*2)=16M/16=1M ,select 2.4V vref
    writeReg32(GPADC_MAIN_CTL, 0x09);     // 配置寄存器 GPADC_MAIN_CTL 打开GPADC模块，，数据采集上升沿
    writeReg32(GPADC_TIMER0, 0x10);       // 通道切换等待时间
    if (adc_get_calibration(&calibration_param) < 0) {
        Serial1.printf("gadc_calibration_get success fail \r\n");
        calibration_param = 10000;
    }
    Serial1.printf("calibration_param = %d \r\n", calibration_param);
}

void adc_gpio_config(uint8_t channel)
{
    gpio_mux_ctl(adc_Pins[channel], 0);
    gpio_fun_sel(adc_Pins[channel], GPIO_Dx);
    gpio_mode_config(adc_Pins[channel], GPIO_INPUT);
}

void adc_config_channel(uint16_t channel)
{
    writeReg32(GPADC_CHAN_CTL, channel);
    writeReg32(GPADC_FIFO_CTL , 0x10);
    writeReg32(GPADC_FIFO_CTL , 0x00);
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
    uint32_t result = 0;
	while(!flag) {
		readReg32(GPADC_FIFO, val);
		if ((((val >> 26) & 0xF) == ch) && (ch == ((val >> 10) & 0xF))) {
			result = (uint32_t)(val & 0x3FF);
			flag = 1;
		}
	}
   // result = result*calibration_param / CALI_CENTER_VL;
    return result;
}
