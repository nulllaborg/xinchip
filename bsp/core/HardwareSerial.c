#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>
#include "Arduino.h"
#include "ringbuffer.h"
#include "HardwareSerial.h"

void serial0_begin(uint32_t baud);
void serial0_end(void);
int serial0_available(void);
int serial0_peek(void);
uint8_t serial0_read(void);
int serial0_availableForWrite(void);
void serial0_flush(void);
size_t serial0_write(uint8_t c);
size_t serial0_write_buf(uint8_t *s, uint32_t len);
void serial0_printf(char *fmt, ...);

HardwareSerial Serial = {
    .begin = serial0_begin,
    .end = serial0_end,
    .available = serial0_available,
    .peek = serial0_peek,
    .read = serial0_read,
    //int availableForWrite(void);
    .flush = serial0_flush,
    .write = serial0_write,
    .write_buf = serial0_write_buf,
    .printf = serial0_printf
};

#if defined(HAVE_HWSERIAL0)
//  void serialEvent() __attribute__((weak));
//  bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
//  void serialEvent1() __attribute__((weak));
//  bool Serial1_available() __attribute__((weak));
#endif
uint8_t buffer[128];

uint32_t serial0EventRun(uint8_t isr_status)
{
    uint32_t len;
 // uint8_t c;
    switch (isr_status) {
        case UART_IIR_RECV:
            for (int i = 0; i < UART_FIFO_BUF_SIZE; i++) {
                ringbuffer_putchar(&(Serial1.rx_rb), R_UARTx_RBR(0));
            }
            break;
        case UART_IIR_RECV_TIME_OUT:
           len = uart_recv_buf(0, buffer);
           ringbuffer_put(&(Serial1.rx_rb), buffer, len);
            break;
        case UART_IIR_SEND_EMPTY:
            // uart_send_str(1, "isr\r\n");
            // if (ringbuffer_getchar(&(Serial1.tx_rb), &c)) {
            //   uart_send_char(0, c);
            //}
           break;
    }
    return 1;
}

// Public Methods //////////////////////////////////////////////////////////////

void serial0_begin(uint32_t baud)
{
    ringbuffer_init(&Serial.rx_rb, Serial.rx_buffer, SERIAL_RX_BUFFER_SIZE);
    //ringbuffer_init(&Serial.tx_rb, Serial.tx_buffer, SERIAL_TX_BUFFER_SIZE);
    init_uart(0, UART0_TX_PIN, UART0_RX_PIN, baud);
    uart_register_callback(0, serial0EventRun);
}

void serial0_end(void)
{
    ringbuffer_destroy(&Serial.rx_rb);
    //ringbuffer_destroy(&Serial.tx_rb);
}

int serial0_available(void)
{
    return ringbuffer_data_len(&(Serial.rx_rb));
}

int serial0_peek(void)
{
  if (ringbuffer_status(&(Serial.rx_rb)) == RINGBUFFER_FULL) {
    return -1;
  } else {
    return *(Serial.rx_rb.buffer_ptr + Serial.rx_rb.read_index);
  }
}

uint8_t serial0_read(void)
{
    uint8_t ch;
    if (ringbuffer_getchar(&Serial.rx_rb, &ch)) return ch;
    return 0;
}

int serial0_availableForWrite(void)
{
    return 1;
}

void serial0_flush(void)
{
   // while (ringbuffer_status(&(Serial.tx_rb)) == RINGBUFFER_EMPTY);
}

size_t serial0_write(uint8_t c)
{
    // return ringbuffer_putchar(&(Serial.tx_rb), c);
    uart_send_char(1, c);
    return 1;
}

size_t serial0_write_buf(uint8_t *s, uint32_t len)
{
   // return ringbuffer_put(&(Serial.tx_rb), s, len);
    uart_send_buf(1, s, len);
    return 1;
}

void serial0_printf(char *fmt, ...)
{
    va_list v_arg;
    char str[256];
    va_start(v_arg, fmt);
    vsprintf(str, fmt, v_arg);
    serial0_write_buf((uint8_t *)str, strlen(str));
}
