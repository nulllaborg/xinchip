#ifndef _I2C_H_
#define _I2C_H_

#include "board.h"
#include "Arduino.h"
#include "gpio.h"

#define __ASSERT(a)    while(!(a))
#define I2C_STANDARD_MODE     (1)           //- 100K bit/s
#define I2C_FAST_MODE         (2)           //- 400K bit/s

#define I2C_SLAVE_RX_DONE       (1<<7)
#define I2C_SLAVE_RD_REQ        (1<<5)
#define I2C_SLAVE_RX_FULL       (1<<2)
#define I2C_SLAVE_STOP          (1<<9)
typedef	void (*i2cHandler_callback_t)(void);

void i2c_master_init(uint8_t i2cAddR, uint32_t speed);
void i2c_master_write(uint8_t suba, uint8_t idata);
void i2c_master_writemulti(uint8_t suba, uint8_t no, uint8_t *s);
void i2c_master_read(uint8_t suba, uint8_t *s);
void i2c_master_readmulti(uint8_t suba, uint8_t no , uint8_t *s);

void i2c_slave_init(uint8_t sda_pin, uint8_t scl_pin, uint16_t i2cAddR);
void i2c_slave_register_callback(i2cHandler_callback_t callback);
void i2c_slave_write(uint8_t val);
uint8_t i2c_slave_read(void);


#endif
