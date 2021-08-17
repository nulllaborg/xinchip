#ifndef __DMA_H_
#define __DMA_H_

#include "system.h"
#include "gpio.h"

enum {
    DMA_UART0_SEND = 0,
    DMA_UART1_SEND,
    DMA_SPI0_SEND,
    DMA_SPI1_SEND,
    DMA_UART0_REV = 8,
    DMA_UART1_REV,
    DMA_SPI0_REV,
    DMA_SPI1_REV,
    DMA_ADC  // 12
}E_DMA_CH;

void dma_enable(E_DMA_CH ch);
void dma_send(uint8_t ch, uint8_t *dma_buf, uint32_t len);
void dma_recive(uint8_t ch, uint8_t *dma_buf, uint32_t len);

#endif
