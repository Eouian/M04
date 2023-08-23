#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"
#include "SPI.h"
#include "RTC6705.h"

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
	;
}

int main(void)
{
    systick_config();
		SPI_init_();
	  RTC6705_io_init();
		RTC6705_send(0x00000000,400);//R = 400
	  RTC6705_frequency(2900000000);
    while(1)
			{
        gpio_bit_set(GPIOB,GPIO_PIN_5);delay_1ms(1);
				gpio_bit_reset(GPIOB,GPIO_PIN_5);delay_1ms(19);
				
			}
}
