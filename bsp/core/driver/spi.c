#include "gpio.h"
#include "spi.h"

volatile uint8_t rw_val;
uint8_t xc_buffer[42]={0xE6,0xE7,0xE7,0xE7,0xE7};
uint8_t spi_sck, spi_dat, spi_cs;

#ifdef SPI_SOFTWARE

void spi_init(uint8_t sck, uint8_t dat, uint8_t cs)
{
	gpio_mux_ctl(sck, 0);
	gpio_fun_inter(sck, 0);
	gpio_fun_sel(sck, 0);

	gpio_mux_ctl(dat, 0);
	gpio_fun_inter(dat, 0);
	gpio_fun_sel(dat, 0);

	gpio_mux_ctl(cs, 0);
	gpio_fun_inter(cs, 0);
	gpio_fun_sel(cs, 0);

	gpio_mode_config(cs, GPIO_OUTPUT);
	gpio_output_high(cs);
	gpio_mode_config(sck, GPIO_OUTPUT);
	gpio_output_low(sck);
	gpio_output_low(cs);;
}

void spi_write(uint8_t data)
{
	gpio_mode_config(spi_dat, GPIO_OUTPUT);
    for (unsigned char i = 0; i < 8; i++){
		gpio_output_low(spi_sck);
		if(data & 0x80)
			SPI_DATA_High;
		else
            SPI_DATA_Low;
        data = data << 1;
        SCK_High;
    }
    SPI_DATA_High;
    SCK_Low;
}

unsigned char spi_read(void)
{  
    gpio_mode_config(spi_dat, GPIO_INPUT);
    unsigned char data = 0;
    for(unsigned char i = 0; i < 8; i++)
    {
        SCK_Low;
        data = data << 1;
        SCK_High;
        if(SPI_DATA_Status)       
          data |= 0x01;    
    }
    SCK_Low;
    return data;
}
#endif

#ifdef SPI_HARDWARE

void spi_mosi(uint8_t pin, uint8_t mode, uint8_t freq)
{
    gpio_mux_ctl(pin, 0);
	gpio_fun_inter(pin, 0);
	gpio_fun_sel(pin, SSI1_TX);
	
    writeReg32(CPR_SPIx_MCLK_CTL(1), 0x110010);//1分频			//- spi(x)_mclk = 32Mhz(When TXCO=32Mhz).
    writeReg32(CPR_CTLAPBCLKEN_GRCTL , (0x1000100<<1)); 	//- 打开spi(x) pclk.
    writeReg32(CPR_SSI_CTRL, 0x30); // Master SPI PROTOCOL
    writeReg32(SSIx_EN(1), 0x00);   // disable SSI1
    writeReg32(SSIx_IE(1), 0x00);
    writeReg32(SSIx_CTRL0(1) , SPI_TMODE_TX | mode | SPI_MOTOROLA_PROTOCOL | 0x07);					/* 8bit SPI data */
    writeReg32(SSIx_SE(1), 0x01);
    writeReg32(SSIx_BAUD(1), freq);			//- spix_mclk 分频.

    writeReg32(SSIx_TXFTL(1), 0x00);
    writeReg32(SSIx_EN(1) , 0x01);
}

void init_spi_master(uint8_t cs, uint8_t sclk, uint8_t miso, uint8_t mosi, uint8_t mode)
{
    uint32_t val;
    writeReg32(CPR_SPIx_MCLK_CTL(1), 0x110010);//1分频			//- spi(x)_mclk = 32Mhz(When TXCO=32Mhz).
    writeReg32(CPR_CTLAPBCLKEN_GRCTL , (0x1000100<<1)); 	//- 打开spi(x) pclk.
    readReg32(CPR_SSI_CTRL, val);
    val |= (1==0)? 0x01: 0x30;
    writeReg32(CPR_SSI_CTRL, val);
    writeReg32(SSIx_EN(1), 0x00);
    writeReg32(SSIx_IE(1), 0x00);
    writeReg32(SSIx_CTRL0(1) , mode | SPI_MOTOROLA_PROTOCOL | 0x07);					/* 8bit SPI data */
    writeReg32(SSIx_SE(1), 0x01);
    writeReg32(SSIx_BAUD(1), SPIM_CLK_16MHZ);			//- spix_mclk 分频.

    writeReg32(SSIx_RXFTL(1), 0x00);
    writeReg32(SSIx_TXFTL(1), 0x00);

    writeReg32(SSIx_EN(1) , 0x01);
    
    //cs线由GPIO0手动拉低拉高 手动模拟
	gpio_mux_ctl(cs,0);
	gpio_fun_inter(cs,0);
	gpio_fun_sel(cs, 0);
	gpio_mode_config(cs, GPIO_OUTPUT);
	gpio_output_high(cs);
	
    //sclk
	gpio_mux_ctl(sclk, 0);
	gpio_fun_inter(sclk,0);
	gpio_fun_sel(sclk, SSI1_CLK);
	
    //miso
    gpio_mux_ctl(miso, 0);
	gpio_fun_inter(miso, 0);
	gpio_fun_sel(miso, SSI1_RX);
	
    //mosi
    gpio_mux_ctl(mosi,0);
	gpio_fun_inter(mosi,0);
	gpio_fun_sel(mosi, SSI1_TX);
}

uint8_t spi_write(uint8_t dat)
{   
	writeReg32(SSI1_DATA, dat);
	while(((*(SSI1_STS)) & 0x5) != 0x4);
	return (*(SSI1_DATA));
}

uint8_t spi_read(void)
{
	writeReg32(SSI1_DATA, 0x00);
    while(((*(SSI1_STS))&0x9) != 0x8);
    return (*(SSI1_DATA));
}

#endif

uint8_t spi_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t length)
{
  	CS_Low;                    		                                    
  	spi_write(reg);       		                                          
  	for (unsigned char byte_ctr = 0; byte_ctr < length; byte_ctr++) {
		pBuf[byte_ctr] = spi_read();
	}
  	CS_High;
	
	return *pBuf; 
}

void spi_write_buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    CS_Low;
    spi_write(reg);
    for (unsigned char j = 0; j < len; j++) {
		spi_write(pBuf[j]);
	}
    CS_High;
}
