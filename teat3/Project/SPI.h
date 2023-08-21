#ifndef _SPI_H
#define _SPI_H
#include "gd32f1x0.h"
#include "systick.h"

#define SPI_PORTB GPIOB
#define SPI_MOSI GPIO_PIN_4
#define SPI_NSS   GPIO_PIN_3

#define SPI_PORTA GPIOA
#define SPI_SCK  GPIO_PIN_15

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

void SPI_init(void);
void SPI_init_io(void);
void SPI_init_mode(uint8_t spi_mode);

void SPI_begin(void);
void SPI_W_NSS(uint8_t bit_value);

uint8_t SPI_swap(uint8_t byte_to_send);
void SPI_W_SCK(uint8_t bit_value);
void SPI_W_MOSI(uint8_t bit_value);
uint8_t SPI_R_MISO(void);

void SPI_end(void);

#endif