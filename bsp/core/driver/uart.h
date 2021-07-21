#ifndef ___UART_H_
#define ___UART_H_

#include <stdint.h>

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

#if (ENABLE_UART0 == 1)
#define UART0_TX_PIN    18
#define UART0_RX_PIN    19
#define UART0_BAUD      115200
#endif

#if (ENABLE_UART1 == 1)
#define UART0_TX_PIN    18
#define UART0_RX_PIN    19
#define UART0_BAUD      115200
#endif

#define UART_IIR_NONE           0x01
#define UART_IIR_SEND_EMPTY     0x02
#define UART_IIR_RECV           0x04
#define UART_IIR_EEROR          0x06
#define UART_IIR_BUSY           0x07
#define UART_IIR_REC_TIME_OUT   0x0C

typedef uint32_t (*uHandler_callback)(uint8_t ch);

void init_uart(uint8_t ch, uint8_t tx_pin, uint8_t rx_pin, uint32_t baud);
void uart_register_callback(uint8_t ch, uHandler_callback callback);
void uart_send_char(uint8_t ch, uint8_t c);
void uart_send_str(uint8_t ch, uint8_t *s);
void uart_send_buf(uint8_t ch, uint8_t *s, uint32_t length);
void uart_dma_send(uint8_t ch, uint8_t *s, uint32_t length);

#endif
