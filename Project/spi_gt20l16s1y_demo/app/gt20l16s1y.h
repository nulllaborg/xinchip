#ifndef _GT20L16S1Y_H
#define _GT20L16S1Y_H

#include "gpio.h"
#include "spi.h"

void read_ASCII_8x16(uint8_t ascii_code, uint8_t *lattice_buff);
void read_gb2312_16x16(uint32_t gb2312_code, uint8_t *lattice_buff);
#endif
