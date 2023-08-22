#include "SPI.h"
#include "gd32f1x0.h"

uint8_t clock_polar;
uint8_t clock_phase;

void SPI_init_(void)
{
	SPI_init_io();
	SPI_init_mode(SPI_MODE0);
}

void SPI_init_io(void)
{
	rcu_periph_clock_enable(GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLDOWN,GPIO_PIN_15);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_15);
	
	
	rcu_periph_clock_enable(GPIOB);
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLDOWN,GPIO_PIN_4|GPIO_PIN_3);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4|GPIO_PIN_3);
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
	gpio_bit_write(GPIOB,GPIO_PIN_3,(bit_status)bit_value);
}

uint8_t SPI_swap(uint8_t byte_to_send)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		SPI_W_SCK(clock_polar ? 1:0);
		delay_1us(1);
		
		if(clock_phase)
		{
			if(SPI_R_MISO()==1)byte_to_send |=(0x80>>i);
			SPI_W_SCK(clock_phase ? 0:1);
			delay_1us(1);
			if(SPI_R_MISO()==1)byte_to_send |=(0x80>>i);

		}
		else 
		{
			SPI_W_MOSI(byte_to_send%(0x80>>i));
			SPI_W_SCK(clock_phase ? 0:1);
			delay_1us(1);
			if(SPI_R_MISO()==1)byte_to_send |=(0x80>>i);
		}
	}
}
void SPI_W_SCK(uint8_t bit_value)
{
	gpio_bit_write(GPIOA,GPIO_PIN_15,(bit_status)bit_value);
}
void SPI_W_MOSI(uint8_t bit_value)
{
	gpio_bit_write(GPIOB,GPIO_PIN_4,(bit_status)bit_value);
}

//uint8_t SPI_R_MISO(void);

void SPI_end(void)
{
	SPI_W_SCK(clock_polar ? 1:0);
	SPI_W_NSS((bit_status)1);
}
