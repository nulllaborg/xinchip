#ifndef _HardwareSerial_h
#define _HardwareSerial_h

#include "board.h"
#include "ringbuffer.h"
#include "uart.h"

#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 256
#define HAVE_HWSERIAL0 1
#define HAVE_HWSERIAL1 1

typedef struct ST_HardSerial
{
    unsigned char rx_buffer[SERIAL_RX_BUFFER_SIZE];
    //unsigned char tx_buffer[SERIAL_TX_BUFFER_SIZE];
    struct st_ringbuffer rx_rb, tx_rb;
    void (*begin)(uint32_t);
    void (*end)(void);
    int (*available)(void);
    int (*peek)(void);
    size_t (*read)(void);
    size_t (*read_buf)(uint8_t *, uint8_t);
    int (*read_tag)(unsigned int, const char*);
    //int availableForWrite(void);
    void (*flush)(void);
    size_t (*write)(uint8_t);
    size_t (*write_buf)(uint8_t *, uint32_t);
    void (*printf)(char *, ...);
}HardwareSerial;

#ifdef HAVE_HWSERIAL0
extern HardwareSerial Serial;
#endif

#ifdef HAVE_HWSERIAL1
extern HardwareSerial Serial1;
#endif

#endif
