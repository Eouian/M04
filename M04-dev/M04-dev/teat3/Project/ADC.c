#include "gd32f1x0_adc.h"
#include "systick.h"
void ADC_IO_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
}

void ADC_config(void)
{
	rcu_periph_clock_enable(RCU_ADC);//使能ADC时钟
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);//8M
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);//LSB
	adc_channel_length_config(ADC_REGULAR_CHANNEL,1U);
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_NONE);
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	adc_external_trigger_config(ADC_REGULAR_CHANNEL,DISABLE);
	adc_special_function_config(ADC_SCAN_MODE,DISABLE);
	adc_special_function_config(ADC_CONTINUOUS_MODE,DISABLE);
	adc_regular_channel_config(0U,ADC_CHANNEL_1,ADC_SAMPLETIME_239POINT5);//239.5*0.125us
	adc_dma_mode_disable();//DMA
	
	adc_enable();
	delay_1ms(10U);
	adc_calibration_enable();
}

uint16_t ADC_Get_Channel(void)
{
	adc_enable();
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	//while(!adc_flag_get(ADC_FLAG_EOC));
	adc_flag_clear(ADC_FLAG_EOC);
	return adc_regular_data_read();
}
