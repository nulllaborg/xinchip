#include "dma.h"

/* ---------------------------------------------------------------------------------------------------
- 函数名称: dma_send
- 函数功能: Uart用DMA方式发送一个长度为length的uint8_t类型的数组  --注意DMA的只能访问SHRAM0（0x10010000 ~ 0x1001FFFF）
----------------------------------------------------------------------------------------------------*/
static uint8_t __attribute__((aligned(4))) uart_tx_buf[256]={0};

void dma_enable(E_DMA_CH ch)
{
    writeReg32(DMAS_EN, ch);
}

void dma_send(uint8_t ch, uint8_t *dma_buf, uint32_t len)
{

    uint32_t dma_int_status = 0;
    //- TX Channel
    writeReg32(DMAS_CHx_SAR(ch), (uint32_t)dma_buf);   //发送通道起始地址
    writeReg32(DMAS_CHx_DAR(ch), (ch * 0x1000 + 0x40010000));  //发送地址写UART_THR
    writeReg32(DMAS_CHx_CTL1(ch), ((2 << 8)));         //通道优先级2 总线宽度8bit
    writeReg32(DMAS_CHx_CTL0(ch), len);
    dma_enable(ch);
    do	{
        readReg32(DMAS_INT_RAW, dma_int_status);
    }while( (dma_int_status & (1+ch)) != (1 + ch));

    writeReg32(DMAS_INT_RAW, (1+ch));
    writeReg32(DMAS_CLR, ch);
}

void dma_recive(uint8_t ch, uint8_t *dma_buf, uint32_t len)
{
    uint32_t dma_int_status = 0;
    //- RX Channel
    writeReg32(DMAS_CHx_SAR(ch), (ch * 0x1000 + 0x40010000);   //接收通道起始地址
    writeReg32(DMAS_CHx_DAR(ch), (uint32_t)dma_buf));  //接收地址写UART_THR
    writeReg32(DMAS_CHx_CTL1(ch), ((2 << 8)));         //通道优先级2 总线宽度8bit
    writeReg32(DMAS_CHx_CTL0(ch), len);
    dma_enable(ch);
    do	{
        readReg32(DMAS_INT_RAW, dma_int_status);
    }while((dma_int_status & (1 + ch)) != (1 + ch));

    writeReg32(DMAS_INT_RAW, (1+ch));
    writeReg32(DMAS_CLR, ch);
}
