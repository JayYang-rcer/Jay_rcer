#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

typedef struct PID 
{
  float  Proportion;         //  ???? Proportional Const  
  float  Integral;           //  ???? Integral Const  
  float  Derivative;         //  ???? Derivative Const  
	float  PrevError;          //  Error[-2]
  float  LastError;          //  Error[-1]  
	float  Error;
	float  DError;
  float  SumError;           //  Sums of Errors  
	float  Integralmax;
	float  output;
	float  outputmax;
	float  errormax;
	u8 first_flag;
	float  deadzone;
}PID;

void PID_parameter_init(PID *pp, float Kp, float Ki, float Kd, float outputmax, float Integralmax, float deadzone);
float PID_abs_limit(float a, float ABS_MAX);
void PID_incremental_PID_calculation(PID *pp,  float CurrentPoint, float NextPoint);
void PID_incremental_PID_calculation_by_error(PID *pp, float error);
void PID_position_PID_calculation(PID *pp, float CurrentPoint, float NextPoint);
void PID_position_PID_calculation_by_error(PID *pp, float error);
void PID_reset_PID(PID *pp);

#endif
