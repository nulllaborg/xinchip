#ifndef __WS2812B_H
#define __WS2812B_H 

#define SPI_DMA

#define White       0xFFFFFF  // 白色
#define Black       0x000000  // 黑色
#define Green       0x00ff00  // 绿色
#define Red         0xff0000  // 红色
#define Blue        0x0000ff  // 蓝色

typedef struct RGB_RAW
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} ST_RGB;

void set_pixel_color(unsigned long int c);
void ws2812b_reset(void);
void ws2812b_init(void);
#endif
