#ifndef _PID_H_
#define _PID_H_
#include "gd32f1x0.h"

static uint16_t target_mode_1 = 14;
static uint16_t target_mode_2 = 20;
static uint16_t target_mode_3 = 23;
static uint16_t target_mode_4 = 26;

static uint16_t voltage_mode_1 = 1;
static uint16_t voltage_mode_2 = 2;
static uint16_t voltage_mode_3 = 3;
static uint16_t voltage_mode_4 = 4;

typedef struct
{
	float proportion,integration ,differentiation;
	float Error,Error_before;
	float Interation,Interation_max;
	float Output,Output_max;
}PID_parameter;

void PID_init(PID_parameter *pid,
									float p,float i,float d,
									float I_max,float out_max);
void PID_calculate(PID_parameter *pid,float refrence,
										float feedback);
									
#endif
