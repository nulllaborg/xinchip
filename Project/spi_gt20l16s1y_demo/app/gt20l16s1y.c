#include "gt20l16s1y.h"

/* ---------------------------------------------------------------------------------------------------
- 函数名称: read_ASCII_8x16
- 函数功能: 读取字库芯片中一个ASCII字符的 8x16 点阵编码
- 输入参数: ASCII码，lattice_buff:点阵编码缓冲区
----------------------------------------------------------------------------------------------------*/
void read_ASCII_8x16(uint8_t ascii_code, uint8_t *lattice_buff)
{
	uint32_t base_addr = 0x3B7C0;	//8x16点阵的起始地址
	uint32_t addr; 					//ASCII字符点阵在芯片中的位置
	
	if ((ascii_code >= 0x20) && (ascii_code <= 0x7E)) {
		addr = (ascii_code - 0x20) * 16 + base_addr;
	}
	SPI_CS_LOW;
	spi_write(0x0B);
	spi_write((addr >> 16) & 0xFF);
	spi_write((addr >> 8) & 0xFF);
	spi_write((addr >> 0) & 0xFF);
	spi_write(0xFF);
	for (int i = 0; i < 16; i++) {
		*(lattice_buff + i) = spi_read();
	}
	SPI_CS_HIGHT;
}

/* ---------------------------------------------------------------------------------------------------
- 函数名称: read_gb2312_16x16
- 函数功能: 读取字库芯片中一个国标汉字的 16x16 点阵编码
- 输入参数: gb2312_code码，lattice_buff:点阵编码缓冲区
----------------------------------------------------------------------------------------------------*/
void read_gb2312_16x16(uint32_t gb2312_code, uint8_t *lattice_buff)
{
	uint32_t base_addr = 0;			//16x16点阵的起始地址
	uint8_t msb, lsb;				//msb:汉字内码高八位 lsb:汉字内码低八位
	uint32_t addr; 					//汉字点阵在芯片中的位置
	
	msb = (gb2312_code >> 8) & 0xFF;
	lsb = (gb2312_code >> 0) & 0xFF;

	if(msb == 0xA9 && lsb >= 0xA1) {
		addr = (282 + (lsb - 0xA1)) * 32 + base_addr;
	}
	else if(msb >= 0xA1 && msb <= 0xA3 && lsb >= 0xA1) {
		addr = (msb - 0xA1) * 94 + (lsb - 0xA1) * 32 + base_addr;
	}
	else if(msb >= 0xB0 && msb <= 0xF7 && lsb >= 0xA1) {
		addr = ((msb - 0xB0) * 94 + (lsb - 0xA1) + 846) * 32 + base_addr;
	}
	
	SPI_CS_LOW;
	spi_write(0x0B);
	spi_write((addr >> 16) & 0xFF);
	spi_write((addr >> 8) & 0xFF);
	spi_write((addr >> 0) & 0xFF);
	spi_write(0xFF);
	for (int i = 0; i < 32; i++) {
		*(lattice_buff + i) = spi_read();
	}
	SPI_CS_HIGHT;
}

