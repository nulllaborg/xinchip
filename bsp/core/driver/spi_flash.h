#ifndef __BSP_SPI_FLASH_H_
#define __BSP_SPI_FLASH_H_
#include <stdint.h>
#include "spi.h"

// 256K falsh = 4 block = 64 sector = 256 page

#define FLASH_PAGE_SIZE			256	
#define FLASH_BLOCK_SIZE		65536  // 1 block = 16 sector 64K
#define FLASH_SECTOR_SIZE		4096   // 1 sector = 16page  4K


/*----------------------------------------------------------------------------------------------------
    SPI 	COMMAND
-----------------------------------------------------------------------------------------------------*/
#define		CMD_READ_DATA			  0x03
#define		CMD_READ_STATUS			0x05
#define		CMD_CHIP_ERASE			0xc7
#define		CMD_WRITE_ENABLE		0x06
#define		CMD_PAGE_PROGRAM		0x02
#define		CMD_BLOCK_ERASE			0xD8
#define		CMD_SECTOR_ERASE		0x20
#define		CMD_PAGE_ERASE		 	0x81
#define		CMD_RELEASE_PWRDWN	0xAB
#define		CMD_PWRDWN	      	0xB9
#define		CMD_ID          		0x4B 
#define		PACKET_FULL_LEN			FLASH_PAGE_SIZE

void init_spi_flash(uint32_t ch, uint32_t freq);
void spi_flash_read_page(uint32_t	PageAddR, uint8_t *buff);
uint8_t	spi_flash_wait_till_ready(void);
void spi_flash_chip_erase(void);
uint32_t spi_flash_block_num(uint32_t	size);
void spi_flash_block_erase(uint32_t	blknum);
void spi_flash_write_enable(void);
void spi_flash_write_page(uint32_t PageAddR, uint8_t *buff);
void spi_flash_sector_erase(uint32_t no);
void spi_flash_release_powerdown(void);
void spi_flash_enter_powerdown(void);
uint16_t spi1_write_read_uint16(uint16_t val);
void spi1_write_read_stream(uint16_t *input, uint16_t *output, uint16_t length);
void spi_flash_read(uint32_t ReadAddR, uint8_t *buff,uint16_t ReadLength);
void spi_flash_write(uint32_t	WriteAddR, uint8_t *buff,uint16_t WriteLength);
void spi_flash_read_128bitsID(uint8_t *buff);

/*--------------------------------------------------------------
for Example:

	for(i=0; i<256; i++) {
		data_buff[i] = i;
	}
	spi_flash_write_enable();
	spi_flash_block_erase(0);
	while(spi_flash_wait_till_ready());

	spi_flash_write_enable();
	spi_flash_write_page(3, &data_buff[0]);
	while(spi_flash_wait_till_ready());

	for(i=0; i<256; i++)	{
		data_buff[i] = 0;
	}
	spi_flash_Read_Page(3, &data_buff[0]);
--------------------------------------------------------------*/
#endif
