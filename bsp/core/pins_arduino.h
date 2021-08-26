
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define NUM_DIGITAL_PINS    16
#define NUM_ANALOG_INPUTS   8
#define NUM_PWM_PINS   6

#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)

#if defined(__XC610__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

#define PIN_SPI_SS    (9)

#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (18)
#define PIN_WIRE_SCL        (19)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 	(13)

#define PIN_A0   (21)
#define PIN_A1   (20)
#define PIN_A2   (19)
#define PIN_A3   (18)
#define PIN_A4   (0)
#define PIN_A5   (1)
#define PIN_A6   (4)
#define PIN_A7   (5)

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
static const uint8_t A6 = 6;
static const uint8_t A7 = 7;

#define D0  0
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7

#define D8  8
#define D9  9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

//#ifdef ARDUINO_MAIN

static uint8_t boardADCPins[NUM_ANALOG_INPUTS] = {
	21, 20, 19 , 18 , 0 , 1 , 4 , 5
};

static uint16_t port_to_mode_PGM[] = {
//	NOT_A_PORT,
//	NOT_A_PORT,
	0,
	0,
};

static uint8_t boardPWMPins[NUM_PWM_PINS] = {
    21, 20, 0 , 1 , 12 , 13
};



//#endif

#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
