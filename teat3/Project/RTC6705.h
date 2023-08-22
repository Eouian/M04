#ifndef _RTC6705_H_
#define _RTC6705_H_

#include "SPI.h"

void RTC6705_io_init();
void RTC6705_send(uint32_t addr,uint32_t data);
void RTC6705_frequency(uint32_t f);

#endif