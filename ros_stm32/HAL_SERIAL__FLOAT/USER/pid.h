#ifndef  __PID_H
#define  __PID_H

#define  ABS(x)      ((x)>0? (x):(-(x)))

typedef struct PID_Date
{
	float error ;
	float last_error ;
	float earlier_error ;
	float K_P ;
	float K_I ;
	float K_D ;
	float I_Separate ;
	float I_Limit ;
	float Out_MAX ;
	float Out_MIN ; 
	float Dead_Size;
  float Output ;
}PID_Date;


extern PID_Date M3508_PID[4];

float PID_Speed_Calculate(PID_Date * PID  ,float expect,float Encoder_Count);
float PID_Position_Calculate(PID_Date * PID,float expect,float Encoder_Count);
void PID_Parameter_Speed_Init(PID_Date * PID,float Pi,float Ki,float Di,float Dead_Size,float Out_MAX,float I_Limit ,float I_Separate );
#endif



