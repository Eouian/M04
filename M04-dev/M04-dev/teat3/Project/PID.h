#ifndef _PID_H_
#define _PID_H_
#include "gd32f1x0.h"



typedef struct
{
	float proportion,integration ,differentiation;//P、I、D值
	float Error,Error_before;
	float Interation,Interation_max;
	float Output,Output_max;
	float target;
	float ActuatorOutput;
}PID_parameter;

void set_target(PID_parameter *pid,uint16_t level);
void PID_init(PID_parameter *pid,
									float p,float i,float d,
									float I_max,float out_max);
void PID_calculate(PID_parameter *pid,
										float feedback);
									
#endif
