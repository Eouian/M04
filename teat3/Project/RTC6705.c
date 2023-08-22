#include "gd32f1x0.h"
#include "RTC6705.h"
#include "SPI.h"

void RTC6705_io_init()
{
	//VREF
	rcu_periph_clock_enable(GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_0);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
	gpio_bit_set(GPIOA,GPIO_PIN_0);
	//BIAS
	rcu_periph_clock_enable(GPIOB);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_5);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);
	gpio_bit_reset(GPIOB,GPIO_PIN_5);
}

void RTC6705_send(uint32_t addr,uint32_t data)
{
	uint8_t i;
	uint32_t pack;
	pack =0;
	pack |=addr&0x0000000f;
	pack |=0x00000010;
	pack |=data&0x1ffffe0;
	
	SPI_begin();
	for(i=0;i<25;i++)
	{
		if((pack&0x00000001)==1)SPI_W_MOSI((bit_status)1);
		else SPI_W_MOSI((bit_status)0);
		SPI_W_SCK((bit_status)1);
		SPI_W_SCK((bit_status)0);
	}
	SPI_end();
}
void RTC6705_frequency(uint32_t f)
{
	uint32_t N=0;
	uint32_t A=0;
	uint32_t regB=0;
	
	N=f/10000/64;
	A=f/10000-64*N;
	regB |=A;
	regB |=N<<7;
	RTC6705_send(0X01000000,regB);
}
