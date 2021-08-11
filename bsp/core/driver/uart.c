#include "system.h"
#include "gpio.h"
#include "uart.h"

uHandler_callback uHandler_Callback[2] = {(uHandler_callback)0, (uHandler_callback)0};

/* ---------------------------------------------------------------------------------------------------
- init_uart
- function: init serial rx,tx pin name and baud
----------------------------------------------------------------------------------------------------*/
void init_uart(uint8_t ch , uint8_t tx_pin, uint8_t rx_pin, uint32_t baud)
{
    uint32_t val;
    gpio_mux_ctl(tx_pin, 0);
    gpio_fun_inter(tx_pin, 0);
    gpio_mux_ctl(rx_pin, 0);
    gpio_fun_inter(rx_pin, 0);
    if (ch == 0) {
        gpio_fun_sel(tx_pin, UART0_TX);
        gpio_fun_sel(rx_pin, UART0_RX);
    } else if (ch == 1){
        gpio_fun_sel(tx_pin, UART1_TX);
        gpio_fun_sel(rx_pin, UART1_RX);
    }

    writeReg32(CPR_RSTCTL_SUBRST_SW, (1 << (16 + ch)));               //复位uart模块
    writeReg32(CPR_RSTCTL_SUBRST_SW, ((1 << (16 + ch)) | ( 1<< ch))); //解复位uart模块

    readReg32(CPR_LP_CTL, val);
    val &= ~(1<<(1-ch));
    writeReg32(CPR_LP_CTL, val);                                      //关闭uartx时钟保护

    val = (1<<(16+ch+4)) | (1<<(ch+4));
    writeReg32(CPR_CTLAPBCLKEN_GRCTL, val);   //打开uartx pclk
    writeReg32(CPR_UARTx_CLK_GRCTL(ch), 0x110018); // DIV8  32M/8 = 4M
    //writeReg32(CPR_UARTx_CLK_CTL(ch), 0x480271);
    writeReg32(CPR_UARTx_CLK_CTL(ch), baud >> 4);

    writeReg32(UARTx_TCR(ch) ,0x80);         //使能DLAB位
    //writeReg32(UARTx_DLL(ch) ,baud);
    writeReg32(UARTx_DLL(ch), baud & 0x0F);
    writeReg32(UARTx_DLH(ch), 0);

    writeReg32(UARTx_TCR(ch), 0x03);        // 清零DLAB位,8个BIT位

    writeReg32(UARTx_FCR(ch), 0x87);        // 接收FIFO半满报中断, 并使能FIFO中断
	// writeReg32(UARTx_IIR(ch), 0xC0);     // enable fifo
    writeReg32(UARTx_IER(ch), 0x81);        // enable rx isr
	//writeReg32(UARTx_IER(ch), 0x83);      // enable tx and rx isr
    val = UART0_IRQn + ch;
    NVIC_EnableIRQ((IRQn_Type)val);
}

void uart_register_callback(uint8_t ch, uHandler_callback callback)
{
    uHandler_Callback[ch] = callback;
}

uint8_t uart_read(uint8_t ch)
{
	while ((R_UARTx_TSR(ch) & UART_RX_RADY) != UART_RX_RADY);
	return (R_UARTx_RBR(ch)&0xFF);
}

uint32_t uart_recv_buf(uint8_t ch, uint8_t *buf)
{
    uint32_t len = 0;
    while((R_UARTx_TSR(ch) & UART_RX_RADY)) {
        *buf++ = R_UARTx_RBR(ch);
        len ++;
    }
    return (len);
}

uint8_t uart_isr_status(uint8_t ch)
{
	uint32_t uart_isr_status = 0;
    readReg32(UARTx_IIR(ch), uart_isr_status);
    return(uart_isr_status&0x0F);
}

void UART0_Handler(void)
{
    uint32_t uart0_isr_status = 0;
	readReg32(UART0_IIR , uart0_isr_status);
	uart0_isr_status &= 0x0F;
    if(uHandler_Callback[0] != (uHandler_callback)0)
    (uHandler_Callback[0])(uart0_isr_status);
}

void UART1_Handler(void)
{
	uint32_t uart1_isr_status = 0;
	readReg32(UART1_IIR , uart1_isr_status);
	uart1_isr_status &= 0x0F;
	if((uart1_isr_status != 0x04) && (uart1_isr_status != 0x0c)) return;
	readReg32(UART1_RBR , uart1_isr_status);
	if (uHandler_Callback[1] != (uHandler_callback)0)
	(uHandler_Callback[1])(uart1_isr_status);
}

void uart_send_char(uint8_t ch, uint8_t c)
{
   while((R_UARTx_TSR(1) & UART_TX_FIFO_IS_FULL) == UART_TX_FIFO_IS_FULL);
   /* unsigned int status;
	for( ; ; ) {			
		readReg32(UARTx_TSR(ch), status);
		status &= 0x20;
		if(status != 0x20) break;
	} */
	writeReg32(UARTx_THR(ch), c);
}

void uart_send_str(uint8_t ch, char *s)
{
    while(*s != '\0') {
        uart_send_char(ch, *s++);
    }
}

void uart_send_buf(uint8_t ch, uint8_t *s, uint32_t length)
{
    while(length--) {
        uart_send_char(ch, *s++);
    }
}

static uint8_t __attribute__((aligned(4))) uart_dma_tx_buf[128] = {0};

void uart_dma_send(uint8_t ch, uint8_t *s, uint32_t length)
{
    uint32_t iWK = 0;
    for(int i = 0; i < length; i++)
        uart_dma_tx_buf[i] = s[i];
    //- TX Channel
    writeReg32(DMAS_CHx_SAR(ch), (uint32_t)uart_dma_tx_buf);
    writeReg32(DMAS_CHx_DAR(ch), (ch*0x1000 + 0x40010000));//发送地址写UART_THR
    writeReg32(DMAS_CHx_CTL1(ch), ((2 << 8)));//通道优先级2
    writeReg32(DMAS_CHx_CTL0(ch), length);
    writeReg32(DMAS_EN, ch);
    do {
        readReg32(DMAS_INT_RAW, iWK);
    } while((iWK&(1+ch)) != (1+ch));
    writeReg32(DMAS_INT_RAW, (1+ch));
    writeReg32(DMAS_CLR, ch);
}
