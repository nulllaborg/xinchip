
/*----------------------------------------------------------------------------------------------------
	INCLUDE HEADE FILES
----------------------------------------------------------------------------------------------------*/
#include 	"Platform.h"
//#include    "test_config.h"

IoHandler_callback	IoHandler_Callback = (IoHandler_callback)0;
/* ---------------------------------------------------------------------------------------------------
- 		�û���������ϵͳIO����
----------------------------------------------------------------------------------------------------*/
gpio_config_t		gpio_mux_config	 = {
		/*CPR_CTL_MUXCTL1*/
		.mux1_ctl.pad1.gpio15 = 0, 	.mux1_ctl.pad1.gpio14 = 0,	.mux1_ctl.pad1.gpio13 = 0,	.mux1_ctl.pad1.gpio12 = 0,
		.mux1_ctl.pad1.gpio11 = 0, 	.mux1_ctl.pad1.gpio10 = 0,	.mux1_ctl.pad1.gpio9 =  0,	.mux1_ctl.pad1.gpio8 = 0,	
		.mux1_ctl.pad1.gpio7 =  0, 	.mux1_ctl.pad1.gpio6 =  0,	.mux1_ctl.pad1.gpio5 =  0,	.mux1_ctl.pad1.gpio4 = 0,
		.mux1_ctl.pad1.gpio3 =  0, 	.mux1_ctl.pad1.gpio2 =  0,	.mux1_ctl.pad1.gpio1 =  0,	.mux1_ctl.pad1.gpio0 = 0,
		/*CPR_CTL_MUXCTL2*/
		.mux2_ctl.pad2.swd = 0,	    .mux2_ctl.pad2.swk = 0,     .mux2_ctl.pad2.bootctl =0,  .mux2_ctl.pad2.gpio28 = 0,	
		.mux2_ctl.pad2.gpio27 = 0,	.mux2_ctl.pad2.gpio26 = 0,	.mux2_ctl.pad2.gpio25 = 0,	.mux2_ctl.pad2.gpio24 = 0,
		.mux2_ctl.pad2.gpio23 = 0,	.mux2_ctl.pad2.gpio22 = 0,	.mux2_ctl.pad2.gpio21 = 0,	.mux2_ctl.pad2.gpio20 = 0,
		.mux2_ctl.pad2.gpio19 = 0,	.mux2_ctl.pad2.gpio18 = 0,	.mux2_ctl.pad2.gpio17 = 0,	.mux2_ctl.pad2.gpio16 = 0
};




/* ------------------------------------------------------------------------------------------------------------------
- �û���������IO���ÿ��� - 

0:GPIO_Dx   1:UART0_TX  2:UART0_RX      3:UART0_CTS      4:UART0_RTS  5:I2C_SCL     6:I2C_SDA     7:UART1_RX   8:UART1_TX
9:SIM_IO    10:SIM_RST  11:SIM_CLK_OUT  12:PWM0          13:PWM1      14:SSI1_CLK   15:SSI1_SSN   16:SSI1_RX   17:SSI1_TX
-------------------------------------------------------------------------------------------------------------------*/
gpio_fun_sel_config_t   gpio_fun_sel_config = {
        
/*CPR_GPIO_FUN_SEL0*/.fun_sel0.bits.b0004= GPIO_Dx,/*GPIO 0*/.fun_sel0.bits.b0812= GPIO_Dx,/*GPIO 1*/.fun_sel0.bits.b1620= GPIO_Dx,/*GPIO 2*/.fun_sel0.bits.b2428= GPIO_Dx,/*GPIO 3*/
/*CPR_GPIO_FUN_SEL1*/.fun_sel1.bits.b0004= GPIO_Dx,/*GPIO 4*/.fun_sel1.bits.b0812= GPIO_Dx,/*GPIO 5*/.fun_sel1.bits.b1620= GPIO_Dx,/*GPIO 6*/.fun_sel1.bits.b2428= GPIO_Dx,/*GPIO 7*/    
/*CPR_GPIO_FUN_SEL2*/.fun_sel2.bits.b0004= GPIO_Dx,/*GPIO 8*/.fun_sel2.bits.b0812= GPIO_Dx,/*GPIO 9*/.fun_sel2.bits.b1620= GPIO_Dx,/*GPIO10*/.fun_sel2.bits.b2428= GPIO_Dx,/*GPIO11*/
/*CPR_GPIO_FUN_SEL3*/.fun_sel3.bits.b0004= GPIO_Dx,/*GPIO12*/.fun_sel3.bits.b0812= GPIO_Dx,/*GPIO13*/.fun_sel3.bits.b1620= GPIO_Dx,/*GPIO14*/.fun_sel3.bits.b2428= GPIO_Dx,/*GPIO15*/
/*CPR_GPIO_FUN_SEL4*/.fun_sel4.bits.b0004= GPIO_Dx,/*GPIO16*/.fun_sel4.bits.b0812= GPIO_Dx,/*GPIO17*/.fun_sel4.bits.b1620=UART0_TX,/*GPIO18*/.fun_sel4.bits.b2428=UART0_RX,/*GPIO19*/
/*CPR_GPIO_FUN_SEL5*/.fun_sel5.bits.b0004= GPIO_Dx,/*GPIO20*/.fun_sel5.bits.b0812= GPIO_Dx,/*GPIO21*/.fun_sel5.bits.b1620= GPIO_Dx,/*GPIO22*/.fun_sel5.bits.b2428= GPIO_Dx,/*GPIO23*/
/*CPR_GPIO_FUN_SEL6*/.fun_sel6.bits.b0004= GPIO_Dx,/*GPIO24*/.fun_sel6.bits.b0812= GPIO_Dx,/*GPIO25*/.fun_sel6.bits.b1620= GPIO_Dx,/*GPIO26*/.fun_sel6.bits.b2428= GPIO_Dx,/*GPIO27*/
/*CPR_GPIO_FUN_SEL7*/.fun_sel7.bits.b0004= GPIO_Dx,/*GPIO28*/.fun_sel7.bits.b0812= GPIO_Dx,/*GPIO29*/.fun_sel7.bits.b1620= GPIO_Dx,/*GPIO30*/.fun_sel7.bits.b2428= GPIO_Dx /*GPIO31*/   
    
};

/* ---------------------------------------------------------------------------------------------------
- �û���������ϵͳIO���ж�ģʽ NOT_INT(���ж�)  RIS_EDGE_INT(�������ж�)  FAIL_EDGE_INT(�½����ж�)
----------------------------------------------------------------------------------------------------*/
interrupt_config_t	interrupt_config = {
/*GPIO_INTR_CTRL0*/.intr_ctl0.pad.mode3=NOT_INT,/*GPIO 3*/.intr_ctl0.pad.mode2=NOT_INT,/*GPIO 2*/.intr_ctl0.pad.mode1=NOT_INT,/*GPIO 1*/.intr_ctl0.pad.mode0=NOT_INT,/*GPIO 0*/
/*GPIO_INTR_CTRL1*/.intr_ctl1.pad.mode3=NOT_INT,/*GPIO 7*/.intr_ctl1.pad.mode2=NOT_INT,/*GPIO 6*/.intr_ctl1.pad.mode1=NOT_INT,/*GPIO 5*/.intr_ctl1.pad.mode0=NOT_INT,/*GPIO 4*/
/*GPIO_INTR_CTRL2*/.intr_ctl2.pad.mode3=NOT_INT,/*GPIO11*/.intr_ctl2.pad.mode2=NOT_INT,/*GPIO10*/.intr_ctl2.pad.mode1=NOT_INT,/*GPIO 9*/.intr_ctl2.pad.mode0=NOT_INT,/*GPIO 8*/
/*GPIO_INTR_CTRL3*/.intr_ctl3.pad.mode3=NOT_INT,/*GPIO15*/.intr_ctl3.pad.mode2=NOT_INT,/*GPIO14*/.intr_ctl3.pad.mode1=NOT_INT,/*GPIO13*/.intr_ctl3.pad.mode0=NOT_INT,/*GPIO12*/	
/*GPIO_INTR_CTRL4*/.intr_ctl4.pad.mode3=NOT_INT,/*GPIO19*/.intr_ctl4.pad.mode2=NOT_INT,/*GPIO18*/.intr_ctl4.pad.mode1=NOT_INT,/*GPIO17*/.intr_ctl4.pad.mode0=NOT_INT,/*GPIO16*/
/*GPIO_INTR_CTRL5*/.intr_ctl5.pad.mode3=NOT_INT,/*GPIO23*/.intr_ctl5.pad.mode2=NOT_INT,/*GPIO22*/.intr_ctl5.pad.mode1=NOT_INT,/*GPIO21*/.intr_ctl5.pad.mode0=NOT_INT,/*GPIO20*/
/*GPIO_INTR_CTRL6*/.intr_ctl6.pad.mode3=NOT_INT,/*GPIO27*/.intr_ctl6.pad.mode2=NOT_INT,/*GPIO26*/.intr_ctl6.pad.mode1=NOT_INT,/*GPIO25*/.intr_ctl6.pad.mode0=NOT_INT,/*GPIO24*/
/*GPIO_INTR_CTRL7*/.intr_ctl7.pad.mode3=NOT_INT,/*GPIO31*/.intr_ctl7.pad.mode2=NOT_INT,/*GPIO30*/.intr_ctl7.pad.mode1=NOT_INT,/*GPIO29*/.intr_ctl7.pad.mode0=NOT_INT,/*GPIO28*/		
};                                                                                                                                                                  

/* ---------------------------------------------------------------------------------------------------
- ��������: Init_gpio
- ��������: Gpioģ���ʼ��
- �������: ��
- ��������: 2016-05-24
----------------------------------------------------------------------------------------------------*/
extern	void	Init_gpio(void)
{
	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL , 0x40004); /*gpio_pclk enable*/
	__write_hw_reg32(CPR_OTHERCLKEN_GRCTL , 0x10001);  /*gpio_clk  enable*/       
	__write_hw_reg32(CPR_GPIO_FUN_SEL0 , gpio_fun_sel_config.fun_sel0.config); 
	__write_hw_reg32(CPR_GPIO_FUN_SEL1 , gpio_fun_sel_config.fun_sel1.config);	
	__write_hw_reg32(CPR_GPIO_FUN_SEL2 , gpio_fun_sel_config.fun_sel2.config);
	__write_hw_reg32(CPR_GPIO_FUN_SEL3 , gpio_fun_sel_config.fun_sel3.config);	    
	__write_hw_reg32(CPR_GPIO_FUN_SEL4 , gpio_fun_sel_config.fun_sel4.config);
	__write_hw_reg32(CPR_GPIO_FUN_SEL5 , gpio_fun_sel_config.fun_sel5.config);	    
	__write_hw_reg32(CPR_GPIO_FUN_SEL6 , gpio_fun_sel_config.fun_sel6.config);
	__write_hw_reg32(CPR_GPIO_FUN_SEL7 , gpio_fun_sel_config.fun_sel7.config);	
	__write_hw_reg32(CPR_CTL_MUXCTL1 , gpio_mux_config.mux1_ctl.muxctl1);
	__write_hw_reg32(CPR_CTL_MUXCTL2 , gpio_mux_config.mux2_ctl.muxctl2);			
	__write_hw_reg32(GPIO_INTR_CTRL0 , (0xF0000|(interrupt_config.intr_ctl0.interrupt)));	__write_hw_reg32(GPIO_INTR_CTRL1 , (0xF0000|(interrupt_config.intr_ctl1.interrupt)));
	__write_hw_reg32(GPIO_INTR_CTRL2 , (0xF0000|(interrupt_config.intr_ctl2.interrupt)));	__write_hw_reg32(GPIO_INTR_CTRL3 , (0xF0000|(interrupt_config.intr_ctl3.interrupt)));
	__write_hw_reg32(GPIO_INTR_CTRL4 , (0xF0000|(interrupt_config.intr_ctl4.interrupt)));	__write_hw_reg32(GPIO_INTR_CTRL5 , (0xF0000|(interrupt_config.intr_ctl5.interrupt)));
	__write_hw_reg32(GPIO_INTR_CTRL6 , (0xF0000|(interrupt_config.intr_ctl6.interrupt)));	__write_hw_reg32(GPIO_INTR_CTRL7 , (0xF0000|(interrupt_config.intr_ctl7.interrupt)));	
	NVIC_EnableIRQ(GPIO_IRQn);
}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_mux_pin_ctl
- ��������: pin�����ӿ���
- �������: mux
- ��������: 2019-12-03
- 48 PIN��
- GPIO0 : ��mux=0 ���ӵ�gpio_d[0] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO1 : ��mux=0 ���ӵ�gpio_d[1] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO2 : ��mux=0 ���ӵ�gpio_d[2] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO3 : ��mux=0 ���ӵ�gpio_d[3] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO4 : ��mux=0 ���ӵ�gpio_d[4] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO5 : ��mux=0 ���ӵ�gpio_d[5] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO6 : ��mux=0 ���ӵ�gpio_d[6] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO7 : ��mux=0 ���ӵ�gpio_d[7] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO8 : ��mux=0 ���ӵ�gpio_d[8] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO9 : ��mux=0 ���ӵ�gpio_d[9] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO10: ��mux=0 ���ӵ�gpio_d[10]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO11: ��mux=0 ���ӵ�gpio_d[11]�� ��mux=1 NA��               ��mux=2 txen��       ��mux=3 NA�� 
- GPIO12: ��mux=0 ���ӵ�gpio_d[12]�� ��mux=1 NA��               ��mux=2 rxen��       ��mux=3 NA�� 
- GPIO13: ��mux=0 ���ӵ�gpio_d[13]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO14: ��mux=0 ���ӵ�gpio_d[14]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO15: ��mux=0 ���ӵ�gpio_d[15]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO16: ��mux=0 ���ӵ�gpio_d[16]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO17: ��mux=0 ���ӵ�gpio_d[17]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO18: ��mux=0 ���ӵ�gpio_d[18]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO19: ��mux=0 ���ӵ�gpio_d[19]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO20: ��mux=0 ���ӵ�gpio_d[20]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO21: ��mux=0 ���ӵ�gpio_d[21]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO22: ��mux=0 ���ӵ�gpio_d[22]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO23: ��mux=0 ���ӵ�gpio_d[23]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO24: ��mux=0 ���ӵ�gpio_d[24]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO25: ��mux=0 ���ӵ�gpio_d[25]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO27: ��mux=0 ���ӵ�gpio_d[27]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO28: ��mux=0 ���ӵ�gpio_d[28]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA�� 
- GPIO29: ��mux=0 ���ӵ�BOOT_CTL  �� ��mux=1 ���ӵ�gpio_d[29]�� ��mux=1 NA��         ��mux=2 NA��
- GPIO30: ��mux=0 ���ӵ�SWI�� SWCK�� ��mux=1 ���ӵ�gpio_d[30]�� ��mux=1 NA��         ��mux=2 NA��
- GPIO31: ��mux=0 ���ӵ�SWI�� SWD �� ��mux=1 ���ӵ�gpio_d[31]�� ��mux=1 NA��         ��mux=2 NA��
- 32/16 PIN��
- GPIO0 : ��mux=0 ���ӵ�gpio_d[0] �� ��mux=1 NA��               ��mux=2 pwm2��       ��mux=3 NA��
- GPIO1 : ��mux=0 ���ӵ�gpio_d[1] �� ��mux=1 NA��               ��mux=2 pwm3��       ��mux=3 NA��
- GPIO2 : ��mux=0 ���ӵ�gpio_d[2] �� ��mux=1 NA��               ��mux=2 clk_12M_out����mux=3 NA��
- GPIO3 : ��mux=0 ���ӵ�gpio_d[3] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO4 : ��mux=0 ���ӵ�gpio_d[4] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO5 : ��mux=0 ���ӵ�gpio_d[5] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO6 : ��mux=0 ���ӵ�gpio_d[6] �� ��mux=1 NA��               ��mux=2 txen��       ��mux=3 NA��
- GPIO7 : ��mux=0 ���ӵ�gpio_d[7] �� ��mux=1 NA��               ��mux=2 rxen��       ��mux=3 NA��
- GPIO8 : ��mux=0 ���ӵ�gpio_d[8] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO9 : ��mux=0 ���ӵ�gpio_d[9] �� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO10: ��mux=0 ���ӵ�gpio_d[10]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO11: ��mux=0 ���ӵ�BOOT_CTL  �� ��mux=1 ���ӵ�gpio_d[11]�� ��mux=2 NA��         ��mux=3 NA��
- GPIO12: ��mux=0 ���ӵ�SWI�� SWCK�� ��mux=1 ���ӵ�gpio_d[12]�� ��mux=2 NA��         ��mux=3 pwm4��
- GPIO13: ��mux=0 ���ӵ�SWI�� SWD �� ��mux=1 ���ӵ�gpio_d[13]�� ��mux=2 NA��         ��mux=3 pwm5��
- GPIO14: ��mux=0 ���ӵ�gpio_d[14]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO18: ��mux=0 ���ӵ�gpio_d[18]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO19: ��mux=0 ���ӵ�gpio_d[19]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO20: ��mux=0 ���ӵ�gpio_d[20]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO21: ��mux=0 ���ӵ�gpio_d[21]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO22: ��mux=0 ���ӵ�gpio_d[22]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO23: ��mux=0 ���ӵ�gpio_d[23]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO24: ��mux=0 ���ӵ�gpio_d[24]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- GPIO25: ��mux=0 ���ӵ�gpio_d[25]�� ��mux=1 NA��               ��mux=2 NA��         ��mux=3 NA��
- ----------------------------------------------------------------------------------------------------*/
extern void gpio_mux_ctl(uint8_t num,uint8_t mux)
{
    if(mux>3) return;
    uint32_t temp=0;
    __read_hw_reg32((CPR_CTL_MUXCTL1+(num/16)),temp);
    temp=(temp&(~(0x3<<((num%16)*2))))|(mux<<((num%16)*2));
    __write_hw_reg32((CPR_CTL_MUXCTL1+(num/16)),temp);    
}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_fun_inter
- ��������: gpio�ж�ģʽ���� 
- �������: gpio���0-28  �ж�����0���ж� 5�������ж� 7�½����ж�
-��inter=0  GPIO�ر��ж�:NOT_INT        ��                       
-��inter=5  GPIO�������ж�:RIS_EDGE_INT ��                        
-��inter=7  GPIO�½����ж�:FAIL_EDGE_INT��
-��inter=9  GPIO�������½����ж�:RIS_FAIL_EDGE_INT��
- ��������: 2019-12-03
----------------------------------------------------------------------------------------------------*/
extern void gpio_fun_inter(uint8_t num,uint8_t inter)
{
   unsigned int temp=0;
   if(inter>0xF) return;
   switch(num%4)
   {
       case 3:__read_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);temp &= 0x0FFF;temp |= ((inter<<12)|0xF0000);__write_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);break;
       case 2:__read_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);temp &= 0xF0FF;temp |= ((inter<< 8)|0xF0000);__write_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);break;
       case 1:__read_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);temp &= 0xFF0F;temp |= ((inter<< 4)|0xF0000);__write_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);break;
       case 0:__read_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);temp &= 0xFFF0;temp |= ((inter    )|0xF0000);__write_hw_reg32((GPIO_INTR_CTRL0+(num/4)),temp);break;
       default:break;   
   }   

}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_fun_sel
- ��������: gpio���ù�������
- �������: gpio���0-28 sel����
-��sel=0   ��ͨGPIO����:GPIO_Dx               ��
-��sel=1   ����0����:UART0_TX                 ��
-��sel=2   ����0����::UART0_RX                �� 
-��sel=3   ����0����:UART0_CTS                ��
-��sel=4   ����0����:UART0_RTS                ��
-��sel=5   I2Cʱ��:I2C_SCL                    �� 
-��sel=6   I2C����:I2C_SDA                    ��
-��sel=7   ����1����:UART1_RX                 ��
-��sel=8   ����1����:UART1_TX                 �� 
-��sel=9   SIM_IO                             ��   
-��sel=10  SIM_RST                            �� 
-��sel=11  SIM_CLK_OUT                        ��                  
-��sel=12  PWM0���:PWM0                      ��  
-��sel=13  PWM1���:PWM1                      ��   
-��sel=14  SPI1ʱ��:SSI1_CLK                  ��
-��sel=15  SPI1Ƭѡ:SSI1_SSN                  ��
-��sel=16  SPI1����:SSI1_RX                   �� 
-��sel=17  SPI1����:SSI1_TX                   ��
-��sel=18  PWM0�������:PWM0_INV(32/16PIN����)��
-��sel=19  PWM1�������:PWM1_INV(32/16PIN����)��
- ��������: 2019-12-03
----------------------------------------------------------------------------------------------------*/
extern void gpio_fun_sel(uint8_t num,uint8_t sel)
{
   
   unsigned int temp=0;
   if(sel>19) return;
   switch(num%4)
   {
       case 0:__read_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);temp &= 0xFFFFFF00;temp |= sel;      __write_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);break;
       case 1:__read_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);temp &= 0xFFFF00FF;temp |= (sel<< 8);__write_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);break;
       case 2:__read_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);temp &= 0xFF00FFFF;temp |= (sel<<16);__write_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);break;
       case 3:__read_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);temp &= 0x00FFFFFF;temp |= (sel<<24);__write_hw_reg32((CPR_GPIO_FUN_SEL0+(num/4)),temp);break;
       default:break;   
   }   
}


/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_Register_Callback
- ��������: Ϊgpioע���жϻص�����
- �������: �ж�״ֵ̬
- ��������: 2016-05-26
----------------------------------------------------------------------------------------------------*/
extern	void	gpio_Register_Callback(IoHandler_callback callback)
{
	IoHandler_Callback = callback;
}

/* ---------------------------------------------------------------------------------------------------
- ��������: GPIO_Handler
- ��������: GPIO0�жϴ�����
- �������: ��
- ��������: 2016-05-26
----------------------------------------------------------------------------------------------------*/
extern	void	GPIO_Handler(void)
{
		uint32_t	val;
		__read_hw_reg32(GPIO_INTR_STATUS_C00 , val);/*read interrupt flag*/
		__write_hw_reg32(GPIO_INTR_CLR0 , val);     /*clean interrupt flag*/
		if(IoHandler_Callback != (IoHandler_callback)0)
			IoHandler_Callback(val);
      
       if(DETECT_INTER_NUM(GPIO_PIN_24,val))//�ж��Ƿ�ΪGPIO24�ж�
       {
          printf("GPIO24 interrupt\n");
       }
       printf("interrupt:%#x\n",val);
}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_output_high
- ��������: Gpio����ߵ�ƽ
- �������: gpio��� (< 29)
- ��������: 2016-05-24
----------------------------------------------------------------------------------------------------*/
extern	void	gpio_output_high(uint8_t num)
{
		__write_hw_reg32((GPIO_PORT_DR0 + (num>>4)), (0x00010001<<(num&0x0F)));
}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_output_low
- ��������: Gpio����͵�ƽ
- �������: gpio���  (< 29)
- ��������: 2016-05-24
----------------------------------------------------------------------------------------------------*/
extern	void	gpio_output_low(uint8_t	num)
{
		__write_hw_reg32((GPIO_PORT_DR0 + (num>>4)), (0x00010000<<(num&0x0F)));
}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_input_val
- ��������: ��ȡGPIO�ܽ�״̬
- �������: gpio���
- �������: �ߵ͵�ƽֵ
- ��������: 2016-06-07
----------------------------------------------------------------------------------------------------*/
extern	uint8_t gpio_input_val(uint8_t num)
{
		uint32_t val;	
		__read_hw_reg32((GPIO_EXT_PORT0 + (num>>5)), val);
		return (val & (0x01L << (num & 0x1F)))? (1) : (0);
}
/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_direction_output
- ��������: ����GPIO�ܽŷ���Ϊ���
- �������: gpio��� 
- ��������: 2016-06-07
----------------------------------------------------------------------------------------------------*/
extern	void	gpio_direction_output(uint8_t num)
{
		__write_hw_reg32((GPIO_PORT_DDR0 + (num>>4)), ((0x10001)<<(num&0x0F)));

}

/* ---------------------------------------------------------------------------------------------------
- ��������: gpio_direction_input
- ��������: ����GPIO�ܽŷ���Ϊ����
- �������: gpio��� , ������״̬(0�������� 1�������� ������������������
-��pull_up_type=0  GPIO����:GPIO_Mode_Input_Up   ��                       
-��pull_up_type=1  GPIO����:GPIO_Mode_Input_Down ��                        
-��pull_up_type=2  GPIO����:GPIO_Mode_Input_Float��
- ��������: 2016-06-07
----------------------------------------------------------------------------------------------------*/
extern	void		gpio_direction_input(uint8_t num, uint8_t pull_up_type)
{
	    uint8_t temp_num=num;
        uint32_t val;
		uint32_t* base_reg = 0x0;
        //uint32_t pos = (num&0x0f)<<1;

		if(num>=0 && num<=4)
		{
			base_reg = (uint32_t*)(0x40002400+0x38);
		}
		else if(num>=5 && num<=15)
		{
			base_reg = (uint32_t*)(0x40002400+0x34);
            #ifdef PIN_32_16
            if(num==11)     {num=13;base_reg = (uint32_t*)(0x40002400+0x38);}
            else if(num==12){num=14;base_reg = (uint32_t*)(0x40002400+0x38);}
            else if(num==13){num=15;base_reg = (uint32_t*)(0x40002400+0x38);}
            #endif
		}
		else if(num>=16 && num<=28)
		{
			base_reg = (uint32_t*)(0x40002400+0x38);
			num = num%16;
		}
		
        __read_hw_reg32(base_reg , val);
		if(pull_up_type == 1)//����
		{
			setbit(val,num*2);
			clrbit(val,num*2+1);
		}
		else if(pull_up_type == 0)//����
        {
        	clrbit(val,num*2);
            setbit(val,num*2+1);
        }
		
		else//��������
		{
			clrbit(val,num*2);
            clrbit(val,num*2+1);
		}
        __write_hw_reg32(base_reg, val);
		
		__write_hw_reg32((GPIO_PORT_DDR0 + (temp_num>>4)), ((0x10000)<<(temp_num&0x0F)));
		__write_hw_reg32((GPIO_DEBOUNCE0 + (temp_num>>4)), ((0x10001)<<(temp_num&0x0F)));	
}
void GPIO_Sleep_Config(void)
{
   
    //����������
	__write_hw_reg32((volatile unsigned *)(0x40002400+0x34),0x55555555); //pectrl1 = 0x55555555
	__write_hw_reg32((volatile unsigned *)(0x40002400+0x38),0x55555555); //pectrl2 = 0x55555555
// 	__write_hw_reg32((volatile unsigned *)(0x40002400+0x3c),0xF);        //spi0 flash�����������ã���������û�©��200��΢����
    
    //�ر�����GPIO�ж�
    __write_hw_reg32(GPIO_INTR_CTRL0,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL1,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL2,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL3,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL4,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL5,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL6,0xF0000);
    __write_hw_reg32(GPIO_INTR_CTRL7,0xF0000);
    //������GPIO�ܽ����ó���������
	for(int i=0;i<32;i++)
	{
		gpio_direction_input(i,1);
	}
    //������GPIO�ܽ����ó���ͨIO��
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xC0),0x0);
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xC4),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xC8),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xCC),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xD0),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xD4),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xD8),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0xDC),0x0);	 
	//���ӿ��� mux=0
	__write_hw_reg32((volatile unsigned *)(0x40000000+0x128),0x0);	  
	__write_hw_reg32((volatile unsigned *)(0x40000000+0x12C),0x0);	 
}
