#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "binary.h"

#ifdef __cplusplus
extern "C"{
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3
#define INPUT_ANALOG 0x4

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL2V47 1
#define AVDD 2

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


typedef unsigned int word;

#define bit(b) (1UL << (b))
#define bitclear(b) ~(1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void init(void);
void initVariant(void);

int atexit(void (*func)()) __attribute__((weak));

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
void digitalToggle(uint8_t);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, uint8_t);
void analogReadResolution(uint8_t);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogToPin(A) boardADCPins[A]
#define digitalPinToPort(P) (P>>4)
#define digitalPinToBitMask(P) (1 << (P&0x0F))
#define digitalPinToTimer(P) ((P))

#define portOutputRegister(P) (GPIO_PORT_DR0 + (P))
#define portInputRegister(P) (GPIO_EXT_PORT0 + (P>>1))
#define portModeRegister(P) (GPIO_PORT_DDR0 + (P))

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1

#define NOT_ON_TIMER 0x00
#define TIMER0    0x10
#define TIMER0A   0x10
#define TIMER0B   0x11
#define TIMER0AX  0x12
#define TIMER0BX  0x13

#define TIMER1    0x20
#define TIMER1A   0x20
#define TIMER1B   0x21
#define TIMER1C   0x22
#define TIMER1AX  0x23
#define TIMER1BX  0x24

#define TIMER2    0x30
#define TIMER2A   0x31
#define TIMER2B   0x32
#define TIMER2AX  0x33
#define TIMER2BX  0x34

#define TIMER3    0x40
#define TIMER3A   0x40
#define TIMER3B   0x41
#define TIMER3C   0x42
#define TIMER3AX  0x43
#define TIMER3BX  0x44
#define TIMER3AA  0x45

#ifdef __cplusplus
} // extern "C"
#endif

#include "pins_arduino.h"

void sysClock(uint8_t mode);
void sysClockPrescale(uint8_t div);
void sysClockOutput(uint8_t enable);

#ifndef nop
#define	nop()   __asm__ __volatile__ ("nop" ::)
#endif

#endif
