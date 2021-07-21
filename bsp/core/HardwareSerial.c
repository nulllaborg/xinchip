#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"

#if defined(HAVE_HWSERIAL0)
//  void serialEvent() __attribute__((weak));
//  bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
//  void serialEvent1() __attribute__((weak));
//  bool Serial1_available() __attribute__((weak));
#endif

void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL0)
  //if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
  //if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
}

// Public Methods //////////////////////////////////////////////////////////////

void begin(ST_HardSerial *Serial, unsigned long baud)
{
    ringbuffer_init(&Serial->rx_rb, Serial->rx_buffer, SERIAL_RX_BUFFER_SIZE);
    ringbuffer_init(&Serial->tx_rb, Serial->tx_buffer, SERIAL_TX_BUFFER_SIZE);
    init_uart(0, baud);
    uart_register_callback(0, ringbuffer_putchar, &(Serial->rx_rb));
}

void end()
{
    
}

int available(ST_HardSerial *Serial)
{
    return (ringbuffer_space_len(&(Serial->rx_rb));
}

int peek(ST_HardSerial *Serial)
{
  if (ringbuffer_status(&(Serial->rx_rb)) == RINGBUFFER_FULL) {
    return -1;
  } else {
    return *(Serial->rx_rb.buffer_ptr + Serial->rx_rb.read_index);
  }
}

uint8_t read(ST_HardSerial *Serial)
{
    return ringbuffer_getchar(&(Serial->rx_rb));
}

int availableForWrite(void)
{

}

void flush()
{
    while(ringbuffer_status(&(Serial->tx_rb)) == RINGBUFFER_EMPTY));
}

size_t HardwareSerial::write(uint8_t c)
{
    return ringbuffer_putchar(&(Serial->tx_rb), c);
}

#endif // whole file
