#include "gd32f1x0.h"
#include "RTC6705.h"
#include "SPI.h"


void RTC6705_frequency(uint32_t f_to_compute);
void RTC6705_io_init(void)
{
	//VREF
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_0);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
	gpio_bit_set(GPIOA,GPIO_PIN_0);
	//BIAS
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_5);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);
	gpio_bit_reset(GPIOB,GPIO_PIN_5);
}

void RTC6705_send(uint32_t addr,uint32_t data)
{
	uint8_t i;
	uint32_t pack;
	pack =0;
	pack |=(addr&0x0000000f);
	pack |=0x00000010;
	pack |=(data&0x000fffff)<<5;
	
	
	gpio_bit_reset(GPIOB,GPIO_PIN_3);

	for(i=0;i<25;i++)
	{
		if((pack&0x00000001)==1)gpio_bit_set(GPIOB,GPIO_PIN_4);
		else gpio_bit_reset(GPIOB,GPIO_PIN_4);
		gpio_bit_set(GPIOA,GPIO_PIN_15);
		gpio_bit_reset(GPIOA,GPIO_PIN_15);
		pack>>=1;
	}
	gpio_bit_set(GPIOB,GPIO_PIN_3);
	
}
void RTC6705_frequency(uint32_t f_to_compute)//32->4,294,967,295
																						 //f_to_compute = f/2
{
	uint32_t N=0;
	uint32_t A=0;
	uint32_t regB=0;
	
	N=f_to_compute/20000/64;
	A=f_to_compute/20000-64*N;
	regB |=A;
	regB |=N<<7;
	RTC6705_send(0x0000001,regB);   
}
