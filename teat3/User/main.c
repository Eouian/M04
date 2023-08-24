#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"
#include "SPI.h"
#include "RTC6705.h"

uint16_t const VTX_Freq[]=
{
	5865,5845,5825,5805,5785,5765,5745,5725,//A
	5733,5752,5771,5790,5809,5828,5847,5866,//B
	5705,5685,5665,5645,5885,5905,5925,5945,//E
	5740,5760,5780,5800,5820,5840,5860,5880,//F
	5658,5695,5732,5769,5806,5843,5880,5917,//R
	5362,5399,5436,5473,5510,5547,5584,5621,//L
};
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

void RTC6705_io_init(void);
void RTC6705_send(uint32_t addr,uint32_t data);
void RTC6705_frequency(uint32_t f_to_compute);
void Key_set_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);

	gpio_mode_set(GPIOA,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_10);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_2);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);
}

int main(void)
{
    systick_config();
		SPI_init_();
		Key_set_init();
	  RTC6705_io_init();
		RTC6705_send(0x00000000,400);//R = 400
	  RTC6705_frequency(2900000000);
		gpio_bit_set(GPIOA,GPIO_PIN_2);
    while(1)
			{
				uint8_t i=0;
        gpio_bit_set(GPIOB,GPIO_PIN_5);delay_1ms(1);
				gpio_bit_reset(GPIOB,GPIO_PIN_5);delay_1ms(19);
				
				if(gpio_input_bit_get(GPIOA,GPIO_PIN_10)==RESET)
				{
					if(i>=48)i=0;
					RTC6705_frequency((VTX_Freq[i]*1000000)/2);
					gpio_bit_reset(GPIOA,GPIO_PIN_2);
					delay_1ms(60);
					while(gpio_input_bit_get(GPIOA,GPIO_PIN_10)==RESET);
					gpio_bit_set(GPIOA,GPIO_PIN_2);
					i++;
				}
			}
}
