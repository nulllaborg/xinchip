#include "board.h"
#include "gpio.h"

gpio_config_t gpio_mux_config = {
    /*CPR_CTL_MUXCTL1*/
    .mux1_ctl.pad1.gpio15 = 0, .mux1_ctl.pad1.gpio14 = 0, .mux1_ctl.pad1.gpio13 = 0, .mux1_ctl.pad1.gpio12 = 0,
    .mux1_ctl.pad1.gpio11 = 0, .mux1_ctl.pad1.gpio10 = 0, .mux1_ctl.pad1.gpio9 =  0, .mux1_ctl.pad1.gpio8 = 0,
    .mux1_ctl.pad1.gpio7 =  0, .mux1_ctl.pad1.gpio6 =  0, .mux1_ctl.pad1.gpio5 =  0, .mux1_ctl.pad1.gpio4 = 0,
    .mux1_ctl.pad1.gpio3 =  0, .mux1_ctl.pad1.gpio2 =  0, .mux1_ctl.pad1.gpio1 =  0, .mux1_ctl.pad1.gpio0 = 0,
    /*CPR_CTL_MUXCTL2*/
    .mux2_ctl.pad2.swd = 0,    .mux2_ctl.pad2.swk = 0,    .mux2_ctl.pad2.bootctl = 0, .mux2_ctl.pad2.gpio28 = 0,
    .mux2_ctl.pad2.gpio27 = 0, .mux2_ctl.pad2.gpio26 = 0, .mux2_ctl.pad2.gpio25 = 0, .mux2_ctl.pad2.gpio24 = 0,
    .mux2_ctl.pad2.gpio23 = 0, .mux2_ctl.pad2.gpio22 = 0, .mux2_ctl.pad2.gpio21 = 0, .mux2_ctl.pad2.gpio20 = 0,
    .mux2_ctl.pad2.gpio19 = 0, .mux2_ctl.pad2.gpio18 = 0, .mux2_ctl.pad2.gpio17 = 0, .mux2_ctl.pad2.gpio16 = 0
};

/* ------------------------------------------------------------------------------------------------------------------
- 用户配置如下IO复用控制 - 
0:GPIO_Dx   1:UART0_TX  2:UART0_RX      3:UART0_CTS 4:UART0_RTS  5:I2C_SCL     6:I2C_SDA     7:UART1_RX   8:UART1_TX
9:SIM_IO    10:SIM_RST  11:SIM_CLK_OUT  12:PWM0     13:PWM1      14:SSI1_CLK   15:SSI1_SSN   16:SSI1_RX   17:SSI1_TX
-------------------------------------------------------------------------------------------------------------------*/
gpio_fun_sel_config_t gpio_fun_sel_config = {

/*CPR_GPIO_FUN_SEL0*/.fun_sel0.bits.b0004 = GPIO_Dx,/*GPIO 0*/.fun_sel0.bits.b0812 = GPIO_Dx,/*GPIO 1*/.fun_sel0.bits.b1620 = GPIO_Dx,/*GPIO 2*/.fun_sel0.bits.b2428 = GPIO_Dx,/*GPIO 3*/
/*CPR_GPIO_FUN_SEL1*/.fun_sel1.bits.b0004 = GPIO_Dx,/*GPIO 4*/.fun_sel1.bits.b0812 = GPIO_Dx,/*GPIO 5*/.fun_sel1.bits.b1620 = GPIO_Dx,/*GPIO 6*/.fun_sel1.bits.b2428 = GPIO_Dx,/*GPIO 7*/
/*CPR_GPIO_FUN_SEL2*/.fun_sel2.bits.b0004 = GPIO_Dx,/*GPIO 8*/.fun_sel2.bits.b0812 = GPIO_Dx,/*GPIO 9*/.fun_sel2.bits.b1620 = GPIO_Dx,/*GPIO10*/.fun_sel2.bits.b2428 = GPIO_Dx,/*GPIO11*/
/*CPR_GPIO_FUN_SEL3*/.fun_sel3.bits.b0004 = GPIO_Dx,/*GPIO12*/.fun_sel3.bits.b0812 = GPIO_Dx,/*GPIO13*/.fun_sel3.bits.b1620 = GPIO_Dx,/*GPIO14*/.fun_sel3.bits.b2428 = GPIO_Dx,/*GPIO15*/
/*CPR_GPIO_FUN_SEL4*/.fun_sel4.bits.b0004 = GPIO_Dx,/*GPIO16*/.fun_sel4.bits.b0812 = GPIO_Dx,/*GPIO17*/.fun_sel4.bits.b1620 = UART0_TX,/*GPIO18*/.fun_sel4.bits.b2428 = UART0_RX,/*GPIO19*/
/*CPR_GPIO_FUN_SEL5*/.fun_sel5.bits.b0004 = GPIO_Dx,/*GPIO20*/.fun_sel5.bits.b0812 = GPIO_Dx,/*GPIO21*/.fun_sel5.bits.b1620 = GPIO_Dx,/*GPIO22*/.fun_sel5.bits.b2428 = GPIO_Dx,/*GPIO23*/
/*CPR_GPIO_FUN_SEL6*/.fun_sel6.bits.b0004 = GPIO_Dx,/*GPIO24*/.fun_sel6.bits.b0812 = GPIO_Dx,/*GPIO25*/.fun_sel6.bits.b1620 = GPIO_Dx,/*GPIO26*/.fun_sel6.bits.b2428 = GPIO_Dx,/*GPIO27*/
/*CPR_GPIO_FUN_SEL7*/.fun_sel7.bits.b0004 = GPIO_Dx,/*GPIO28*/.fun_sel7.bits.b0812 = GPIO_Dx,/*GPIO29*/.fun_sel7.bits.b1620 = GPIO_Dx,/*GPIO30*/.fun_sel7.bits.b2428 = GPIO_Dx /*GPIO31*/
    
};

void init_gpio(void)
{
    writeReg32(CPR_CTLAPBCLKEN_GRCTL , 0x40004); 
    // CTL_APBPclkCmd(GPIO_PCLK, TRUE); /*gpio_pclk enable*/
    writeReg32(CPR_OTHERCLKEN_GRCTL, 0x10001);  /*gpio_clk  enable*/
    writeReg32(CPR_GPIO_FUN_SEL0, gpio_fun_sel_config.fun_sel0.config); 
    writeReg32(CPR_GPIO_FUN_SEL1, gpio_fun_sel_config.fun_sel1.config);
    writeReg32(CPR_GPIO_FUN_SEL2, gpio_fun_sel_config.fun_sel2.config);
    writeReg32(CPR_GPIO_FUN_SEL3, gpio_fun_sel_config.fun_sel3.config);
    writeReg32(CPR_GPIO_FUN_SEL4, gpio_fun_sel_config.fun_sel4.config);
    writeReg32(CPR_GPIO_FUN_SEL5, gpio_fun_sel_config.fun_sel5.config);
    writeReg32(CPR_GPIO_FUN_SEL6, gpio_fun_sel_config.fun_sel6.config);
    writeReg32(CPR_GPIO_FUN_SEL7, gpio_fun_sel_config.fun_sel7.config);
    writeReg32(CPR_CTL_MUXCTL1, gpio_mux_config.mux1_ctl.muxctl1);
    writeReg32(CPR_CTL_MUXCTL2, gpio_mux_config.mux2_ctl.muxctl2);
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: gpio_mux_pin_ctl
- 函数功能: pin脚连接控制
- 输入参数: mux
- 创建日期: 2019-12-03
- 48 PIN脚
- GPIO0 : 【mux=0 连接到gpio_d[0] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO1 : 【mux=0 连接到gpio_d[1] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO2 : 【mux=0 连接到gpio_d[2] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO3 : 【mux=0 连接到gpio_d[3] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO4 : 【mux=0 连接到gpio_d[4] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO5 : 【mux=0 连接到gpio_d[5] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO6 : 【mux=0 连接到gpio_d[6] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO7 : 【mux=0 连接到gpio_d[7] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO8 : 【mux=0 连接到gpio_d[8] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO9 : 【mux=0 连接到gpio_d[9] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO10: 【mux=0 连接到gpio_d[10]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO11: 【mux=0 连接到gpio_d[11]】 【mux=1 NA】               【mux=2 txen】       【mux=3 NA】 
- GPIO12: 【mux=0 连接到gpio_d[12]】 【mux=1 NA】               【mux=2 rxen】       【mux=3 NA】 
- GPIO13: 【mux=0 连接到gpio_d[13]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO14: 【mux=0 连接到gpio_d[14]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO15: 【mux=0 连接到gpio_d[15]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO16: 【mux=0 连接到gpio_d[16]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO17: 【mux=0 连接到gpio_d[17]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO18: 【mux=0 连接到gpio_d[18]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO19: 【mux=0 连接到gpio_d[19]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO20: 【mux=0 连接到gpio_d[20]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO21: 【mux=0 连接到gpio_d[21]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO22: 【mux=0 连接到gpio_d[22]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO23: 【mux=0 连接到gpio_d[23]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO24: 【mux=0 连接到gpio_d[24]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO25: 【mux=0 连接到gpio_d[25]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO27: 【mux=0 连接到gpio_d[27]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO28: 【mux=0 连接到gpio_d[28]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】 
- GPIO29: 【mux=0 连接到BOOT_CTL  】 【mux=1 连接到gpio_d[29]】 【mux=1 NA】         【mux=2 NA】
- GPIO30: 【mux=0 连接到SWI的 SWCK】 【mux=1 连接到gpio_d[30]】 【mux=1 NA】         【mux=2 NA】
- GPIO31: 【mux=0 连接到SWI的 SWD 】 【mux=1 连接到gpio_d[31]】 【mux=1 NA】         【mux=2 NA】
- 32/16 PIN脚
- GPIO0 : 【mux=0 连接到gpio_d[0] 】 【mux=1 NA】               【mux=2 pwm2】       【mux=3 NA】
- GPIO1 : 【mux=0 连接到gpio_d[1] 】 【mux=1 NA】               【mux=2 pwm3】       【mux=3 NA】
- GPIO2 : 【mux=0 连接到gpio_d[2] 】 【mux=1 NA】               【mux=2 clk_12M_out】【mux=3 NA】
- GPIO3 : 【mux=0 连接到gpio_d[3] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO4 : 【mux=0 连接到gpio_d[4] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO5 : 【mux=0 连接到gpio_d[5] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO6 : 【mux=0 连接到gpio_d[6] 】 【mux=1 NA】               【mux=2 txen】       【mux=3 NA】
- GPIO7 : 【mux=0 连接到gpio_d[7] 】 【mux=1 NA】               【mux=2 rxen】       【mux=3 NA】
- GPIO8 : 【mux=0 连接到gpio_d[8] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO9 : 【mux=0 连接到gpio_d[9] 】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO10: 【mux=0 连接到gpio_d[10]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO11: 【mux=0 连接到BOOT_CTL  】 【mux=1 连接到gpio_d[11]】 【mux=2 NA】         【mux=3 NA】
- GPIO12: 【mux=0 连接到SWI的 SWCK】 【mux=1 连接到gpio_d[12]】 【mux=2 NA】         【mux=3 pwm4】
- GPIO13: 【mux=0 连接到SWI的 SWD 】 【mux=1 连接到gpio_d[13]】 【mux=2 NA】         【mux=3 pwm5】
- GPIO14: 【mux=0 连接到gpio_d[14]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO18: 【mux=0 连接到gpio_d[18]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO19: 【mux=0 连接到gpio_d[19]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO20: 【mux=0 连接到gpio_d[20]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO21: 【mux=0 连接到gpio_d[21]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO22: 【mux=0 连接到gpio_d[22]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO23: 【mux=0 连接到gpio_d[23]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO24: 【mux=0 连接到gpio_d[24]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- GPIO25: 【mux=0 连接到gpio_d[25]】 【mux=1 NA】               【mux=2 NA】         【mux=3 NA】
- ----------------------------------------------------------------------------------------------------*/
 void gpio_mux_ctl(uint8_t pin, uint8_t mux)
{
    uint8_t bitmask = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    if (mux > 3) return;
    clearBitReg32((CPR_CTL_MUXCTL1 + port), 3 << bitmask*2);
    setBitReg32((CPR_CTL_MUXCTL1 + port), mux << bitmask*2);
}

void gpio_output_high(uint8_t pin)
{
	volatile uint32_t *out;
	uint8_t bitmask = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	if (port > 2) return;
	out = portOutputRegister(port);
	writeReg32(out, (0x00010001 << bitmask));
}

void gpio_output_low(uint8_t pin)
{
	volatile uint32_t *out;
	uint8_t port = digitalPinToPort(pin);
	uint8_t bitmask = digitalPinToBitMask(pin);
	if (port > 2) return;
	out = portOutputRegister(port);
	writeReg32(out, (0x00010000 << bitmask));
}

uint8_t gpio_input_val(uint8_t pin)
{
	volatile uint32_t *read_reg;
	uint32_t val;
	uint8_t port = digitalPinToPort(pin);
	uint8_t bitmask = digitalPinToBitMask(pin);
	if (port > 2) 
		return 0;
	read_reg = portInputRegister(port);
	readReg32(read_reg, val);
	if (val & bit(bitmask)) 
		return 1;
	else 
		return 0;
}

void gpio_mode_config(uint8_t pin, uint8_t mode)
{
    uint8_t bit;
    uint8_t port = digitalPinToPort(pin);
    volatile uint32_t *mod_reg, *pull_reg, *debounce_reg;
    mod_reg = portModeRegister(port);
    debounce_reg = GPIO_DEBOUNCE0 + port;
    if (port > 2) return;
    if (pin < 15) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x34);
        bit = pin;
    } else if (pin >= 15 && pin < 31) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x38);
        bit = pin&0xF;
    } else if (pin >= 32 && pin <= 34) {
        pull_reg = (volatile unsigned *)(CPR_AO_BASE + 0x3C);
        bit = 2 + pin&0xF;
    }
    if(mode == GPIO_OUTPUT) {
        setBitReg32(mod_reg, (0x10001<<(pin&0x0F)));
        return;
    }

    if (mode == GPIO_INPUT) {
        clearBitReg32(pull_reg, 1<< bit*2);
        clearBitReg32(pull_reg, 1<<(bit*2+1));
    } else if (mode == GPIO_INPUT_PULLUP) {
        setBitReg32(pull_reg, 1 << (bit * 2 + 1));
        clearBitReg32(pull_reg, 1 << bit*2);
    } else if (mode == GPIO_INPUT_PULLDOWN) {
        setBitReg32(pull_reg, 1 << bit * 2);
        clearBitReg32(pull_reg, 1 << (bit*2 + 1));
    }
        writeReg32(mod_reg, (0x10000<<(pin&0x0F)));
        writeReg32(debounce_reg, ((0x10001)<<(pin&0x0F)));
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: gpio_fun_sel
- 函数功能: gpio复用功能设置
- 输入参数: gpio编号0-28 sel复用
-【sel=0   普通GPIO功能:GPIO_Dx               】
-【sel=1   串口0发送:UART0_TX                 】
-【sel=2   串口0接收::UART0_RX                】
-【sel=3   串口0流控:UART0_CTS                】
-【sel=4   串口0流控:UART0_RTS                】
-【sel=5   I2C时钟:I2C_SCL                    】
-【sel=6   I2C数据:I2C_SDA                    】
-【sel=7   串口1接收:UART1_RX                 】
-【sel=8   串口1发送:UART1_TX                 】
-【sel=9   SIM_IO                             】
-【sel=10  SIM_RST                            】
-【sel=11  SIM_CLK_OUT                        】
-【sel=12  PWM0输出:PWM0                      】
-【sel=13  PWM1输出:PWM1                      】
-【sel=14  SPI1时钟:SSI1_CLK                  】
-【sel=15  SPI1片选:SSI1_SSN                  】
-【sel=16  SPI1接收:SSI1_RX                   】
-【sel=17  SPI1发送:SSI1_TX                   】
-【sel=18  PWM0互补输出:PWM0_INV(32/16PIN新增)】
-【sel=19  PWM1互补输出:PWM1_INV(32/16PIN新增)】
- 创建日期: 2019-12-03
----------------------------------------------------------------------------------------------------*/

void gpio_fun_sel(uint8_t pin, uint8_t fun)
{
    uint8_t bitmask = pin%4;
    uint8_t port = pin/4;
    if (fun > 19) return;
    clearBitReg32((CPR_GPIO_FUN_SEL0 + port), 3 << bitmask*8);
    setBitReg32((CPR_GPIO_FUN_SEL0 + port), fun << bitmask*8);
}

void gpio_fun_inter(uint8_t pin, uint8_t inter)
{
    uint8_t bitmask = pin%4;
    uint8_t port = pin/4;
    if (inter > 0xF) return;
    clearBitReg32((GPIO_INTR_CTRL0 + port), 1 << bitmask*4);
    setBitReg32((CPR_GPIO_FUN_SEL0 + port), inter << bitmask*4);
}

void gpio_sleep_config(void)
{
    //配置上下拉
    writeReg32((volatile unsigned *)(0x40002400+0x34),0x55555555); //pectrl1 = 0x55555555
    writeReg32((volatile unsigned *)(0x40002400+0x38),0x55555555); //pectrl2 = 0x55555555
    // writeReg32((volatile unsigned *)(0x40002400+0x3c),0xF);        //spi0 flash脚上下拉配置（如果不配置会漏电200多微安）

    //关闭所有GPIO中断
    writeReg32(GPIO_INTR_CTRL0, 0xF0000);
    writeReg32(GPIO_INTR_CTRL1, 0xF0000);
    writeReg32(GPIO_INTR_CTRL2, 0xF0000);
    writeReg32(GPIO_INTR_CTRL3, 0xF0000);
    writeReg32(GPIO_INTR_CTRL4, 0xF0000);
    writeReg32(GPIO_INTR_CTRL5, 0xF0000);
    writeReg32(GPIO_INTR_CTRL6, 0xF0000);
    writeReg32(GPIO_INTR_CTRL7, 0xF0000);
    //将所有GPIO管脚配置成输入下拉
    for (int i=0;i<32;i++) {
        gpio_mode_config(i, GPIO_INPUT);
    }
    //将所有GPIO管脚配置成普通IO口
    writeReg32((volatile unsigned *)(0x40000000+0xC0),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xC4),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xC8),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xCC),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xD0),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xD4),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xD8),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0xDC),0x0);
    //连接控制 mux=0
    writeReg32((volatile unsigned *)(0x40000000+0x128),0x0);
    writeReg32((volatile unsigned *)(0x40000000+0x12C),0x0);
}
