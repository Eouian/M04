#include "PID.h"
#include "gd32f1x0.h"

//四个功率档位
const uint16_t voltage_mode[4]=
{
		0x01D6,0x02EE,0x03C0,0x04D8
};//14,20,23,26功率档对应的电压值
																  //470,750,960,1240

void PID_init(PID_parameter *pid,float p,float i,float d,
																 float I_max,float out_max)
{
	pid->proportion = p;
	pid->integration = i;
	pid->differentiation = d;
	pid->Interation_max = I_max;
	pid->Output_max = out_max;
}

void set_target(PID_parameter *pid,uint16_t level)
{
	if(level > 3)level = 3;
	else pid->target = voltage_mode[level];
}

void PID_calculate(PID_parameter *pid,float feedback)
{
	pid->Error = pid->target - feedback;
	float p = pid->Error*pid->proportion;
	pid->Interation += pid->Error;
	float i = pid->Interation*pid->integration;
	float d = (pid->Error-pid->Error_before)*pid->differentiation;
	if(pid->Interation > pid->Interation_max)pid->Interation = pid->Interation_max;
	else if(pid->Interation < -pid-> Interation_max)pid->Interation = ADC-pid->Interation_max;
	pid->Output = p + i + d;
	if(pid->Output > pid->Output_max)pid->Output = pid->Output_max;
	else if(pid->Output < -pid->Output_max)pid->Output = -pid->Output_max;
	pid->Error_before = pid->Error;
	pid->ActuatorOutput += pid->Output; 
}
