#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"
#include "SPI.h"
#include "RTC6705.h"
#include "PWM.h"
#include "ADC.h"
#include "PID.h"

//array of frequency
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
uint16_t Val,target_mode;
PID_parameter pid_voltage;
float feedbackVal = 0;
uint16_t num = 0,output = 0,ActuatorOutput = 0;

int main(void)
{
    systick_config();
		//frequency
		SPI_init_();
	  RTC6705_io_init();//VREF, BIAS
		RTC6705_send(0x00000000,400);//REGA, R = 400
	  RTC6705_frequency(2900000000);//5.8G
		//PWM
		PWM_Output(2-1, 2000-1, 198);//PSC, ARR, VAL
		//ADC
		ADC_IO_init();//Vpd
		ADC_config();
		//PID
		PID_init(&pid_voltage,0.06,0,0.01,0,30);//p, i, d, I_max, O_max
		set_target(&pid_voltage,3);//PID目标值设置
		
    while(1)
			{
				
				delay_1ms_user(50);
				Val=ADC_Get_Channel();
				feedbackVal = Val;//将此刻的电压值作为PID的输入值
				num ++;
				PID_calculate(&pid_voltage,feedbackVal);//进行一次PID运算
				ActuatorOutput = pid_voltage.ActuatorOutput;
				output = pid_voltage.Output;
				timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, pid_voltage.ActuatorOutput);//PID运算结果作用于PWM
				
			}
}
