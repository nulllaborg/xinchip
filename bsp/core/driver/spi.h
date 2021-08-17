#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>

#define SPI_MODE0 (0x00 << 6)
#define SPI_MODE1 (0x10 << 6)
#define SPI_MODE2 (0x01 << 6)
#define SPI_MODE3 (0x11 << 6)

#define SPI_MOTOROLA_PROTOCOL (0x00 << 4)
#define SPI_TI_PROTOCOL (0x01 << 4)

#define SPI_TMODE_RX_TX 0
#define SPI_TMODE_TX (0x01 << 8)
#define SPI_TMODE_RX (0x10 << 8)

#define LSBFIRST 0  // 低字节在前
#define MSBFIRST 1  // 高字节在前

#define SPIM_CLK_1MHZ  			32
#define SPIM_CLK_2MHZ  			16
#define SPIM_CLK_4MHZ  			8
#define SPIM_CLK_8MHZ  			4
#define SPIM_CLK_16MHZ 			2

//#define SPIM_CLK_32MHZ 			1 

//#define SPI_SOFTWARE

#define SPI_SCK_GPIO_PIN          3
#define SPI_DATA_GPIO_PIN         25
#define SPI_CS_GPIO_PIN           24

#define SCK_High                  gpio_output_high(SPI_SCK_GPIO_PIN)
#define SCK_Low                   gpio_output_low(SPI_SCK_GPIO_PIN)
#define SCK_Output                gpio_direction_output(SPI_SCK_GPIO_PIN)

#define SPI_DATA_Output           gpio_direction_output(SPI_DATA_GPIO_PIN)
#define SPI_DATA_High             gpio_output_high(SPI_DATA_GPIO_PIN)
#define SPI_DATA_Low              gpio_output_low(SPI_DATA_GPIO_PIN)

#define SPI_DATA_Status           gpio_input_val(SPI_DATA_GPIO_PIN)
#define SPI_DATA_Input            gpio_direction_input(SPI_DATA_GPIO_PIN, 0) //0上拉 1下拉    //gpio_input_val(15)

#define CS_Low		              gpio_output_low(SPI_CS_GPIO_PIN) 
#define CS_High		              gpio_output_high(SPI_CS_GPIO_PIN)
#define CS_Output	              gpio_direction_output(SPI_CS_GPIO_PIN)

#define CS_B_Low		          do{while(((*(SSI1_STS))&0x05)!=0x04);gpio_output_low(26);}while(0)   //gpio 26
#define CS_B_High		          do{while(((*(SSI1_STS))&0x05)!=0x04);gpio_output_high(26);}while(0) //gpio 26
#define CS_B_Output	          gpio_direction_output(26)

//#define CE_Low					do{ unsigned short cfg_otp;	XC_Read_Buf(CFG_TOP,(uint8_t*)&cfg_otp,2); clrbit(cfg_otp,14);XC_Write_Buf(W_REG | CFG_TOP,(uint8_t *)&cfg_otp,2);}while(0)
//#define CE_High					do{	unsigned short cfg_otp; XC_Read_Buf(CFG_TOP,(uint8_t*)&cfg_otp,2); setbit(cfg_otp,14);XC_Write_Buf(W_REG | CFG_TOP,(uint8_t *)&cfg_otp,2);}while(0)

#define CE_Low					do{ unsigned char cfg_otp[2];	XC_Read_Buf(CFG_TOP, cfg_otp,2); clrbit_array(cfg_otp,14);XC_Write_Buf(W_REG | CFG_TOP, cfg_otp,2);}while(0)
#define CE_High					do{	unsigned char cfg_otp[2]; XC_Read_Buf(CFG_TOP, cfg_otp,2); setbit_array(cfg_otp,14);XC_Write_Buf(W_REG | CFG_TOP, cfg_otp,2);}while(0)

#define CE_Output     gpio_direction_output(23)

void spi_mosi(uint8_t pin, uint8_t mode, uint8_t freq);
uint8_t spi_write(uint8_t dat);
#endif
