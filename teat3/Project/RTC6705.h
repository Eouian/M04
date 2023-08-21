#ifndef _RTC6705_H_
#define _RTC6705_H_
#define SPI_SE GPIO_PIN_1
#define SPI_DATA GPIO_PIN_5
#define SPI_LE GPIO_PIN_6
#define SPI_CLK GPIO_PIN_7

void RTC6705_init(void);
void RTC6705_receive(void);
void RTC6705_send(void);

#endif