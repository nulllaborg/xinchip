#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>
#include "Arduino.h"
#include "ringbuffer.h"
#include "HardwareSerial.h"

void serial1_begin(uint32_t baud);
void serial1_end(void);
int serial1_available(void);
int serial1_peek(void);
size_t serial1_read(void);
int serial1_read_tag(unsigned int timeout, const char* tag);
size_t serial1_read_buf(uint8_t *buf, uint8_t len);
int serial1_availableForWrite(void);
void serial1_flush(void);
size_t serial1_write(uint8_t c);
size_t serial1_write_buf(uint8_t *s, uint32_t len);
void serial1_printf(char *fmt, ...);

HardwareSerial Serial1 = {
    .begin = serial1_begin,
    .end = serial1_end,
    .available = serial1_available,
    .peek = serial1_peek,
    .read = serial1_read,
    .read_tag = serial1_read_tag,
    .read_buf = serial1_read_buf,
    //int availableForWrite(void);
    .flush = serial1_flush,
    .write = serial1_write,
    .write_buf = serial1_write_buf,
    .printf = serial1_printf
};

#if defined(HAVE_HWserial1)
//  void serialEvent() __attribute__((weak));
//  bool serial1_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
//  void serialEvent1() __attribute__((weak));
//  bool Serial1_available() __attribute__((weak));
#endif
static uint8_t buffer[128];

uint32_t serial1EventRun(uint8_t isr_status)
{
    uint32_t len;
    // uint8_t c;
    switch (isr_status) {
        case UART_IIR_RECV:
            for (int i = 0; i < UART_FIFO_BUF_SIZE; i++) {
                ringbuffer_putchar(&(Serial1.rx_rb), uart_read(1));
            }
            break;
        case UART_IIR_RECV_TIME_OUT:
           len = uart_recv_buf(1, buffer);
           ringbuffer_put(&(Serial1.rx_rb), buffer, len);
            break;
      case UART_IIR_SEND_EMPTY:
            // uart_send_str(1, "isr\r\n");
            // if (ringbuffer_getchar(&(Serial1.tx_rb), &c)) {
            //   uart_send_char(1, c);
            //}
           break;
    }
    return 1;
}

// Public Methods //////////////////////////////////////////////////////////////

void serial1_begin(uint32_t baud)
{
    ringbuffer_init(&Serial1.rx_rb, Serial1.rx_buffer, SERIAL_RX_BUFFER_SIZE);
    //ringbuffer_init(&Serial1.tx_rb, Serial1.tx_buffer, SERIAL_TX_BUFFER_SIZE);
    init_uart(1, UART1_TX_PIN, UART1_RX_PIN, baud);
    uart_register_callback(1, serial1EventRun);
    for (int i = 0; i < 10000; i++);
    //uart_send_str(1, "begin\r\n");
}

void serial1_end(void)
{
    ringbuffer_destroy(&Serial1.rx_rb);
    //ringbuffer_destroy(&Serial1.tx_rb);
}

int serial1_available(void)
{
    return ringbuffer_data_len(&(Serial1.rx_rb));
}

int serial1_peek(void)
{
  if (ringbuffer_status(&(Serial1.rx_rb)) == RINGBUFFER_FULL) {
    return -1;
  } else {
    return *(Serial1.rx_rb.buffer_ptr + Serial1.rx_rb.read_index);
  }
}

size_t serial1_read(void)
{
    uint8_t c;
    if (ringbuffer_getchar(&Serial1.rx_rb, &c)) return c;
    return 0;
}

size_t serial1_read_buf(uint8_t *buf, uint8_t len)
{
    return ringbuffer_get(&Serial1.rx_rb, buf, len);
}


int serial1_availableForWrite(void)
{
    return 1;
}

void serial1_flush(void)
{
    //while (ringbuffer_status(&(Serial1.tx_rb)) == RINGBUFFER_EMPTY);
}

size_t serial1_write(uint8_t c)
{
    //return ringbuffer_putchar(&(Serial1.tx_rb), c);
    uart_send_char(1, c);
	  return 1;
}

size_t serial1_write_buf(uint8_t *s, uint32_t len)
{
    uart_send_buf(1, s, len);
   // return ringbuffer_put(&(Serial1.tx_rb), s, len);
		return 1;
}

void serial1_printf(char *fmt, ...)
{
    va_list v_arg;
    char str[256];
    va_start(v_arg, fmt);
    vsprintf(str, fmt, v_arg);
    serial1_write_buf((uint8_t *)str, strlen(str));
}

int serial1_read_tag(unsigned int timeout, const char* tag)
{
  unsigned long start = millis();
	int ret = -1;
	while ((millis() - start < timeout) && ret < 0)
	{
    if (ringbuffer_status(&(Serial1.rx_rb)))
		{
		  //Serial1.printf("find tag 1 \r\n");
			if (tag != NULL)
			{ 	
				//Serial1.printf("find tag 2 \r\n");
				if (ringbuffer_ends_with(&(Serial1.rx_rb), tag))
				{
					//Serial1.printf("find tag 3\r\n");
					ret = 0;
					return 0;
				}
			}
		}
	}
	if (millis() - start >= timeout)
	{
		//Serial1.printf(">>> TIMEOUT >>>");
	}
  return ret;
}
