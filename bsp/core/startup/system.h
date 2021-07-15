#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define CPU_CLOCK   32000000ul      /* Set Processor frequency */
#define US_TICKS    32     //CPU_CLOCK/1000000
#define MS_TICKS    32000  //CPU_CLOCK/1000
#define TICKS_PER_SEC   100ul           /* Set the number of ticks in one second  */ 

#define writeReg32(reg,val)  ((*reg) = (val))
#define readReg32(reg, val)  ((val) = (*reg))  
#define setBitReg32(reg, val)  ((*reg) |= (val))
#define clearBitReg32(reg, val)  ((*reg) &= ~(val))

#define readReg16(reg) (*(volatile unsigned short*)(reg))
#define writeReg16(reg, val) (*(volatile unsigned short*)(reg) = (val))

typedef enum {
    WDT_PCLK = 0,
    RTC_PCLK,
    GPIO_PCLK,
    TIMER_PCLK,
    UART0_PCLK,
    UART1_PCLK,
    QDEC_PCLK,
    KBS_PCLK,
    SSI0_PCLK,
    SSI1_PCLK,
    BT_PCLK,
    I2C_PCLK,
    PWM_PCLK,
    GPADC_PCLK,
    SIM_PCLK
}EM_APB_PCLK_Source;

typedef enum{
    GPIO_Dx = 0,
    UART0_TX,
    UART0_RX,
    UART0_CTS,
    UART0_RTS,
    I2C_SCL,
    I2C_SDA,
    UART1_RX,
    UART1_TX,
    SIM_IO,
    SIM_RST,
    SIM_CLK_OUT,
    PWM0,
    PWM1,
    SSI1_CLK,
    SSI1_SSN,
    SSI1_RX,
    SSI1_TX,
    PWM0_INV,
    PWM1_INV,
}GPIO_FUN_SEL_TypeDef;

void CTL_APBPclkCmd(EM_APB_PCLK_Source source, unsigned char enable);
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
#endif
