#include <stdio.h>
#include <string.h>
#include "HardwareSerial.h"
#include "spi_flash.h"

uint8_t __attribute__((aligned(4))) Wbuff[FLASH_PAGE_SIZE+4];
uint8_t __attribute__((aligned(4))) Rbuff[FLASH_PAGE_SIZE+4];

int main(void) 
{
    uint16_t i;
    Serial1.begin(BAUD_115200);
    init_spi_flash(0, SPIM_CLK_16MHZ);
    spi_flash_release_powerdown();
    /****测试flash任意长度任意位置的读写****/
    uint8_t testWrite[62]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
                             'O','P','Q','R','S','T','U','V','W','X','Y','Z',
                             'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
                             'o','p','q','r','s','t','u','v','w','x','y','z',
                             '0','1','2','3','4','5','6','7','8','9'};

    uint8_t testRead[62]="";
    Serial1.printf("\n############################\n");
    spi_flash_write(127*1024, testWrite, 62); //最大读写4096个字节
    spi_flash_read(127*1024, testRead, 62);
    for (i = 0; i < 62; i++) {
        Serial1.printf(" %c ", testRead[i]);
    } 

    /****读独立的ID序列 该指令可以读取一个芯片已经固化的只读的128位ID****/	
    Serial1.printf("\n-----***----\n");
    uint8_t	ID_buf[16]={0};
    spi_flash_read_128bitsID(ID_buf);	
    for(i=0; i<16; i++) {
        Serial1.printf(" %#x ", ID_buf[i]);
    }
    Serial1.printf("\n-----***----\n");
    spi_flash_enter_powerdown();
    while(1);
}
