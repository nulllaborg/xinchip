
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define NUM_DIGITAL_PINS            16
#define NUM_ANALOG_INPUTS           8

#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)

#if defined(__XC610__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

#define PIN_SPI_SS    (9)          // Redirected in main.cpp at line 77
#define PIN_SPI_SS    (10)

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

#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)
#define PIN_A6   (20)
#define PIN_A7   (21)

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
static const uint8_t A6 = 20;
static const uint8_t A7 = 21;

#define D0	0	/* PD0 */
#define D1	1	/* PD1 */
#define D2	2	/* PD2 */
#define D3	3	/* PD3 */
#define D4	4	/* PD4 */
#define D5	5	/* PD5 */
#define D6	6	/* PD6 */
#define D7	7	/* PD7 */

#define D8	8	/* PB0 */
#define D9	9	/* PB1 */
#define D10	10	/* PB2 */
#define D11	11	/* PB3 */
#define D12	12	/* PB4 */
#define D13	13	/* PB5 */

#define D14	14	/* PC0 */
#define D15	15	/* PC1 */
#define D16	16	/* PC2 */
#define D17	17	/* PC3 */
#define D18	18	/* PC4 */
#define D19	19	/* PC5 */


#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#ifdef ARDUINO_MAIN


const uint16_t port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,

};

const uint8_t digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
