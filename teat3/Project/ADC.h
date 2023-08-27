#ifndef _ADC_H_
#define _ADC_H_
#include "gd32f1x0_adc.h"

void ADC_IO_init(void);
void ADC_config(void);
uint16_t ADC_Get_Channel(void);

#endif
