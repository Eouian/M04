#include "gd32f1x0_adc.h"
#include "systick.h"
void ADC_IO_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
}

void ADC_config(void)
{
	rcu_periph_clock_enable(RCU_ADC);
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);//8M
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);//LSB
	adc_channel_length_config(ADC_REGULAR_CHANNEL,1U);
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_NONE);
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
	adc_enable();
	delay_1ms(1U);
	adc_calibration_enable();
}

uint16_t ADC_Get_Channel(uint8_t channel)
{
	adc_regular_channel_config(0U,channel,ADC_SAMPLETIME_239POINT5);
	while(!adc_flag_get(ADC_FLAG_EOC));
	adc_flag_clear(ADC_FLAG_EOC);
	return (adc_regular_data_read()&0xfff);
}
