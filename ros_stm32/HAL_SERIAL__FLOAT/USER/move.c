#include "move.h"
#include "stm32f4xx.h"
#include "user_can_init.h"
#include "pid.h"


volatile  ROBOT_CHASSIS  ROBOT_CHASSI = {0};
PATH_FOLLOW    Path_Contorl = {0,1};   //计数初始化为 1 



void Robot_Wheels_RPM_calculate(void)
{
	/*
	double COS,SIN;
  	COS = cos (Sensor_Buffer.Gyro * PI /180);
  	SIN = sin (Sensor_Buffer.Gyro * PI /180);
	//世界坐标系转换
  	//转化为转速
	ROBOT_CHASSI.SPEED.Robot_VX  = (ROBOT_CHASSI.SPEED.World_VX * COS - ROBOT_CHASSI.SPEED.World_VY * SIN)*MS_transition_RM;
 	 ROBOT_CHASSI.SPEED.Robot_VY  = (ROBOT_CHASSI.SPEED.World_VX * SIN + ROBOT_CHASSI.SPEED.World_VY * COS)*MS_transition_RM;
	//速度分解
	*/

	/*
	3     0           顺时针
	
  	2     1	
	
	*/
	//*****************************************************运动解算********************************************************************************//
	//******************************************给出车坐标的X和Y方向的速度，计算每一个轮子的转速**************************************************//
	 //四轮
	//ROBOT_CHASSI.SPEED.Robot_VX = 1000;
	//ROBOT_CHASSI.SPEED.Robot_VY = 0;
	ROBOT_CHASSI.Motor_RPM[0] = ROBOT_CHASSI.SPEED.Robot_VX*COS45 - ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_RPM[1] =-ROBOT_CHASSI.SPEED.Robot_VX*COS45 - ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_RPM[2] =-ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_RPM[3] = ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	
	//三轮
//	ROBOT_CHASSI.Motor_RPM[0] = (ROBOT_CHASSI.SPEED.Robot_VX + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM );
//	ROBOT_CHASSI.Motor_RPM[1] = (-ROBOT_CHASSI.SPEED.Robot_VX*COS60 - ROBOT_CHASSI.SPEED.Robot_VY*COS30 +ROBOT_CHASSI.SPEED.World_W*MS_transition_RM); 
//	ROBOT_CHASSI.Motor_RPM[2] = (-ROBOT_CHASSI.SPEED.Robot_VX*COS60 + ROBOT_CHASSI.SPEED.Robot_VY*COS30 +ROBOT_CHASSI.SPEED.World_W*MS_transition_RM);
}


void Robot_Wheels_Adjust()
{
	
	
	//速度分解
	Robot_Wheels_RPM_calculate(); //世界坐标换算成车的坐标，并进行运动学解算
	//*************************每一个电机的目标电流 =PID计算(PID的类型（不同电机对应不同类型的PID的ki di等参数），每一个的目标转速，每一个电机的实际转速 )//
	M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[0] , ROBOT_CHASSI.Motor_RPM[0] ,M3508_CHASSIS_MOTOR_REAL_INFO[0].RPM);
	M3508_CHASSIS_MOTOR_REAL_INFO[1].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[1] , ROBOT_CHASSI.Motor_RPM[1] ,M3508_CHASSIS_MOTOR_REAL_INFO[1].RPM);
	M3508_CHASSIS_MOTOR_REAL_INFO[2].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[2] , ROBOT_CHASSI.Motor_RPM[2] ,M3508_CHASSIS_MOTOR_REAL_INFO[2].RPM);
	M3508_CHASSIS_MOTOR_REAL_INFO[3].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[3] , ROBOT_CHASSI.Motor_RPM[3] ,M3508_CHASSIS_MOTOR_REAL_INFO[3].RPM);

	M3508_Send_Motor_Currents(); //发送can信息使电机转起来
	
	 HAL_Delay(10);
	
}


void Motor_PID_init (void)
{
	PID_Parameter_Speed_Init(&M3508_PID[0],10.0,1.0,0.0 , 8000 ,0 , 0 , 500);  //配置m3508的pid
  	PID_Parameter_Speed_Init(&M3508_PID[1],10.0,1.0,0.0 , 8000 ,0 , 0 , 500);
	PID_Parameter_Speed_Init(&M3508_PID[2],10.0,1.0,0.0 , 8000 ,0 , 0 , 500);	
	PID_Parameter_Speed_Init(&M3508_PID[3],10.0,1.0,0.0 , 8000 ,0 , 0 , 500);
}




