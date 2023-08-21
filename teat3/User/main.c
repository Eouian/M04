#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/




int main(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OTYPE_PP,  GPIO_PIN_2);
    gpio_bit_reset(GPIOA,  GPIO_PIN_2);
		
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OTYPE_PP,  GPIO_PIN_3);
    gpio_bit_reset(GPIOA,  GPIO_PIN_3);

		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OTYPE_PP,  GPIO_PIN_4);
    gpio_bit_reset(GPIOA,  GPIO_PIN_4);

    systick_config();
    
    while(1)
			{
        
        gpio_bit_set(GPIOA, GPIO_PIN_2);
        delay_1ms(200);
				gpio_bit_set(GPIOA, GPIO_PIN_3);
        delay_1ms(200);
				gpio_bit_set(GPIOA, GPIO_PIN_4);
        delay_1ms(200);
				
        gpio_bit_reset(GPIOA, GPIO_PIN_2);
        delay_1ms(200);
				gpio_bit_reset(GPIOA, GPIO_PIN_3);
        delay_1ms(200);
				gpio_bit_reset(GPIOA, GPIO_PIN_4);
        delay_1ms(200);
			}
}
