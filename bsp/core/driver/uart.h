#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include "system.h"
#include "gpio.h"

#define  BAUD_1000000   0x00100011
#define  BAUD_921600    0x24002711
#define  BAUD_460800    0x12002711
#define  BAUD_256000    0x060007D3
#define  BAUD_230400    0x09002711
#define  BAUD_128000    0x030007D3
#define  BAUD_115200    0x04802711
#define  BAUD_57600     0x04802712
#define  BAUD_38400     0x04802713
#define  BAUD_23040     0x04802715
#define  BAUD_19200     0x04802716
#define  BAUD_14400     0x04802718
#define  BAUD_12800     0x04802719
#define  BAUD_9600      0x0480271C
#define  BAUD_4800      0x0240271C
#define  BAUD_2400      0x0120271C

#define UART_FIFO_BUF_SIZE 8
#define UART_TX_THR 0x20   //tx is empty isr
#define UART_TX_FIFO_IS_FULL 0x20
#define UART_RX_RADY 0x01

#define R_UARTx_TSR(a) (*UARTx_TSR(a))
#define R_UARTx_RBR(a) (*UARTx_RBR(a))

#if (ENABLE_UART0 == 1)
#define UART0_TX_PIN    18
#define UART0_RX_PIN    19
#define UART0_BAUD      115200
#endif

#if (ENABLE_UART1 == 1)
#define UART1_TX_PIN    18
#define UART1_RX_PIN    19
#define UART1_BAUD      115200
#endif

#define UART_IIR_NONE           0x01
#define UART_IIR_SEND_EMPTY     0x02
#define UART_IIR_RECV           0x04
#define UART_IIR_EEROR          0x06
#define UART_IIR_BUSY           0x07
#define UART_IIR_RECV_TIME_OUT   0x0C

typedef  uint32_t (*uHandler_callback)(uint8_t);

void init_uart(uint8_t ch, uint8_t tx_pin, uint8_t rx_pin, uint32_t baud);
uint8_t uart_read(uint8_t ch);
uint32_t uart_recv_buf(uint8_t ch, uint8_t *buf);
uint8_t uart_isr_status(uint8_t ch);
void uart_register_callback(uint8_t ch, uHandler_callback callback);
void uart_send_char(uint8_t ch, uint8_t c);
void uart_send_str(uint8_t ch, char *s);
void uart_send_buf(uint8_t ch, uint8_t *s, uint32_t length);
void uart_dma_send(uint8_t ch, uint8_t *s, uint32_t length);

#endif
