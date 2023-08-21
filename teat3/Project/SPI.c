#include "SPI.h"
#include "gd32f1x0.h"

uint8_t clock_polar;
uint8_t clock_phase;

void SPI_init(void)
{
	SPI_init_io();
	SPI_init_mode(SPI_MODE0);
}

void SPI_init_io(void)
{
	rcu_periph_clock_enable(SPI_PORTA);
	gpio_deinit(SPI_PORTA);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLDOWN,SPI_SCK);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,SPI_SCK);
	
	rcu_periph_clock_enable(SPI_PORTB);
	gpio_deinit(SPI_PORTB);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLDOWN,SPI_MOSI|SPI_NSS);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,SPI_MOSI|SPI_NSS);
}

void SPI_init_mode(uint8_t spi_mode)
{
	switch(spi_mode)
	{
		case SPI_MODE0:
			clock_polar = 0;
			clock_phase = 0;
			break;
		case SPI_MODE1:
			clock_polar = 0;
			clock_phase = 1;
			break;
		case SPI_MODE2:
			clock_polar = 1;
			clock_phase = 0;
			break;
		case SPI_MODE3:
			clock_polar = 1;
			clock_phase = 1;
			break;
		default:
			break;
	}
}

void SPI_begin(void)
{
	SPI_W_NSS((bit_status)0);
}
void SPI_W_NSS(uint8_t bit_value)
{
	gpio_bit_write(SPI_PORTB,SPI_NSS,(bit_status)bit_value);
}

uint8_t SPI_swap(uint8_t byte_to_send);
void SPI_W_SCK(uint8_t bit_value)
{
	gpio_bit_write(SPI_PORTA,SPI_SCK,(bit_status)bit_value);
}
void SPI_W_MOSI(uint8_t bit_value)
{
	gpio_bit_write(SPI_PORTB,SPI_MOSI,(bit_status)bit_value);
}

//uint8_t SPI_R_MISO(void);

void SPI_end(void)
{
	SPI_W_SCK(clock_polar ? 1:0);
	SPI_W_NSS((bit_status)1);
}
