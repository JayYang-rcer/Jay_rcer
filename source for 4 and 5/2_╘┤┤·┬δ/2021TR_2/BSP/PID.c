#include "headfiles.h"


// 初始化PID参数
// 位置式有积分限幅 增量式没有积分限幅
// 位置式有初始误差功能 增量式没有初始误差功能
// 位置式和增量式都有死区功能 死区设为负数则死区功能不开启
void PID_parameter_init(PID *pp, float Kp, float Ki, float Kd, float outputmax, float Integralmax, float deadzone)  
{  
		pp->Integralmax = Integralmax;
	  pp->outputmax = outputmax;
	  pp->Proportion = Kp;
	  pp->Integral   = Ki;
	  pp->Derivative = Kd;
    pp->DError = pp->Error = pp->SumError = pp->output = pp->LastError = pp->PrevError = pp->errormax = 0.0f;
		pp->first_flag = 1;
		pp->deadzone = deadzone;
}  


// 重置PID
void PID_reset_PID(PID *pp)
{
    pp->DError = pp->Error = pp->SumError = pp->output = pp->LastError = pp->PrevError = pp->errormax = 0.0f; 
		pp->first_flag = 1;
}


// 对变量进行范围限制
float PID_abs_limit(float a, float ABS_MAX)
{
    if(a > ABS_MAX)
        a = ABS_MAX;
		
    if(a < -ABS_MAX)
        a = -ABS_MAX;
		return a;
}

// 增量式PID
void PID_incremental_PID_calculation(PID *pp, float CurrentPoint, float NextPoint)  
{  
	pp->Error =  NextPoint - CurrentPoint;                               
	pp->DError = pp->Error - pp->LastError;
	
	pp->output +=  pp->Proportion * (pp->DError)+   \
								 PID_abs_limit(pp->Integral * pp->Error, pp->Integralmax ) +   \
								 pp->Derivative * ( pp->Error +  pp->PrevError - 2*pp->LastError);  

	if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	if(pp->output < - pp->outputmax )  
		pp->output = -pp->outputmax;
	pp->PrevError = pp->LastError;  
	pp->LastError = pp->Error;
	
	if(ABS(pp->Error) < pp->deadzone)
	{
		pp->output = 0;
	}
}


// 增量式PID,直接传入误差
void PID_incremental_PID_calculation_by_error(PID *pp,  float error)  
{  
	pp->Error = error;                               
	pp->DError = pp->Error - pp->LastError;
	
	pp->output +=  pp->Proportion * (pp->DError)+   \
								 pp->Integral * pp->Error +  \
								 pp->Derivative * ( pp->Error +  pp->PrevError - 2*pp->LastError);  

	if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	if(pp->output < - pp->outputmax )  
		pp->output = -pp->outputmax;
	pp->PrevError = pp->LastError;  
	pp->LastError = pp->Error;
	
	if(ABS(pp->Error) < pp->deadzone)
	{
		pp->output = 0;
	}
}


// 位置式PID
void PID_position_PID_calculation(PID *pp, float CurrentPoint, float NextPoint)  
{   
	if(pp->first_flag == 1)
	{
		pp->LastError = NextPoint - CurrentPoint;
		pp->PrevError = NextPoint - CurrentPoint;
		pp->first_flag = 0;
	}
	
	pp->Error =  NextPoint -  CurrentPoint;          
	pp->SumError += pp->Error;                      
	pp->DError = pp->Error - pp->LastError;
	
	pp->output =  pp->Proportion * pp->Error +   \
								PID_abs_limit(pp->Integral * pp->SumError, pp->Integralmax ) +   \
								pp->Derivative * pp->DError ;  

	if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	if(pp->output < - pp->outputmax )  pp->output = -pp->outputmax; 
	pp->LastError = pp->Error;
	
	if(ABS(pp->Error) < pp->deadzone)
	{
		pp->output = 0;
	}
}


// 位置式PID,直接传入误差
void PID_position_PID_calculation_by_error(PID *pp, float error)  
{   
	if(pp->first_flag == 1)
	{
		pp->LastError = error;
		pp->PrevError = error;
		pp->first_flag = 0;
	}	
	
	pp->Error =  error;          
	pp->SumError += pp->Error;                      
	pp->DError = pp->Error - pp->LastError;
	
	pp->output =  pp->Proportion * pp->Error +   \
								PID_abs_limit(pp->Integral * pp->SumError, pp->Integralmax ) +   \
								pp->Derivative * pp->DError ;  

	if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	if(pp->output < - pp->outputmax )  pp->output = -pp->outputmax; 
	pp->LastError = pp->Error;
	
	if(ABS(pp->Error) < pp->deadzone)
	{
		pp->output = 0;
	}
}
