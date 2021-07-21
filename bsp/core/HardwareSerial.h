#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "ringbuffer.h"
#include "uart.h"

#define SERIAL_TX_BUFFER_SIZE 128
#define SERIAL_RX_BUFFER_SIZE 128

typedef struct ST_HardSerial
{
    bool _written;
    unsigned char rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char tx_buffer[SERIAL_TX_BUFFER_SIZE];
    struct st_ringbuffer rx_rb, tx_rb;
    void begin(ST_HardSerial *hs, unsigned long baud);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    int availableForWrite(void);
    void flush(void);
    size_t write(uint8_t);
    size_t write(unsigned long n);
    size_t write_buf(uint8_t *, long n);
}HardwareSerial;

#ifdef HAVE_HWSERIAL0
extern HardwareSerial Serial;
#endif

#ifdef HAVE_HWSERIAL1
extern HardwareSerial Serial1;
#endif

#endif
