#include "PID.h"
#include "gd32f1x0.h"



void PID_init(PID_parameter *pid,float p,float i,float d,
																 float I_max,float out_max)
{
	pid->proportion = p;
	pid->integration = i;
	pid->differentiation = d;
	pid->Interation_max = I_max;
	pid->Output_max = out_max;
}

void PID_calculate(PID_parameter *pid,float refrence,float feedback)
{
	pid->Error_before = pid->Error;
	pid->Error = refrence - feedback;
	float p = pid->Error*pid->proportion;
	pid->integration += pid->Error*pid->integration;
	float d = (pid->Error-pid->Error_before)*pid->differentiation;
	if(pid->integration > pid->Interation_max)pid->integration = pid->Interation_max;
	else if(pid->integration < -pid-> Interation_max)pid->integration = ADC-pid->Interation_max;
	pid->Output = p + pid->integration + d;
	if(pid->Output > pid->Output_max)pid->Output = pid->Output_max;
	else if(pid->Output < -pid->Output_max)pid->Output = -pid->Output_max;
}
