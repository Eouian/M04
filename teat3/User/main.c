#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"
#include "SPI.h"
#include "RTC6705.h"
#include "PWM.h"
#include "ADC.h"

uint16_t const VTX_Freq[]=
{
	5865,5845,5825,5805,5785,5765,5745,5725,//A
	5733,5752,5771,5790,5809,5828,5847,5866,//B
	5705,5685,5665,5645,5885,5905,5925,5945,//E
	5740,5760,5780,5800,5820,5840,5860,5880,//F
	5658,5695,5732,5769,5806,5843,5880,5917,//R
	5362,5399,5436,5473,5510,5547,5584,5621,//L
};

void RTC6705_io_init(void);
void RTC6705_send(uint32_t addr,uint32_t data);
void RTC6705_frequency(uint32_t f_to_compute);
uint16_t Val;
int main(void)
{
    systick_config();
	//  delay_1ms(1000);
		//frequency
		SPI_init_();
	  RTC6705_io_init();
		RTC6705_send(0x00000000,400);//R = 400
	  RTC6705_frequency(2900000000);
		//PWM
		PWM_Output(4-1, 1000-1, 199);
		//ADC
		ADC_IO_init();
		ADC_config();
    while(1)
			{
				delay_1ms(500);
				Val=ADC_Get_Channel(ADC_REGULAR_CHANNEL);
			}
}
