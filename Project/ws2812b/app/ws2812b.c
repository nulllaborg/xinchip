#include "gpio.h"
#include "ws2812b.h"
#include "Arduino.h"

#ifdef SPI_DMA
#include "spi.h"

#define WS2812B_0_CODE 0xC0
#define WS2812B_1_CODE 0xFC
#endif

#define WS2812_IO  22  
#define NUM_LED 12 //灯的个数

#ifndef SPI_DMA
void ws2812b_set_high()
{
    digitalWrite(WS2812_IO, HIGH);
    delay_us(650);
    digitalWrite(WS2812_IO, LOW);
	 // delay_us(300);
}

void ws2812b_set_low()
{
    digitalWrite(WS2812_IO, HIGH);
   // delay_us(300);
    digitalWrite(WS2812_IO, LOW);
    delay_us(650);
}
void ws2812b_send_24bits(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int c = (r << 16) | (g << 8) | b;
     
    for (int i = 0; i < 24; i++)
    {
        c <<= 1;
        if( (c & 0x800000) == 0x800000)
        {
            ws2812b_set_high();
        }
        else
        {
            ws2812b_set_low();
        }
    }
}

void ws2812b_reset(void)
{
    pinMode(WS2812_IO, OUTPUT);
    digitalWrite(WS2812_IO, LOW);
    delay_us(300);
}

#else 

void ws2812b_init(void)
{
	spi_mosi(22, SPI_MODE3, SPIM_CLK_8MHZ);
}

void ws2812b_reset(void)
{
	spi_write(0);
  delay_us(300);
}

void ws2812b_send_24bits(unsigned char r, unsigned char g, unsigned char b)
{
    for (int i = 0; i < 8; i++)
    {
        r <<= 1;
        if( (r & 0x80) == 0x80)
        {
            spi_write(WS2812B_1_CODE);
        } else {
            spi_write(WS2812B_0_CODE);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        g <<= 1;
        if( (g & 0x80) == 0x80)
        {
            spi_write(WS2812B_1_CODE);
        } else {
            spi_write(WS2812B_0_CODE);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        b <<= 1;
        if( (b & 0x80) == 0x80)
        {
            spi_write(WS2812B_1_CODE);
        } else {
            spi_write(WS2812B_0_CODE);
        }
    }
}


#endif

void set_pixel_color(unsigned long c)
{
    unsigned char r, g, b; 
    r = (unsigned char)(c>>16);
    g = (unsigned char)(c>>8);
    b = (unsigned char)(c);
    for (int i = 0; i < NUM_LED; i++) 
	  {
        ws2812b_send_24bits(r, g, b);
    }
}
