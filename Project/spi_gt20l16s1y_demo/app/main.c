#include <stdio.h>
#include <string.h>
#include "board.h"
#include "gt20l16s1y.h"

uint8_t rx_buff[0x20];
int	main(void)
{
	init_uart(1, GPIO_PIN_2, GPIO_PIN_3, BAUD_115200);
	init_spi_master(GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, SPI_MODE0);
	
	read_ASCII_8x16('A', rx_buff);
	for (int i = 0; i < 16; i++) {
		uart_send_char(1, rx_buff[i]);
	}
	
	read_gb2312_16x16(0xC4E3,rx_buff);
	for (int i = 0; i < 32; i++) {
		uart_send_char(1, rx_buff[i]);
	}
	
    while(1);	
}
