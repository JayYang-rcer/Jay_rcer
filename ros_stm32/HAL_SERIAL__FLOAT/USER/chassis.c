#include "main.h"


ROBOT_CHASSIS  ROBOT_CHASSI = {0};


void Robot_Wheels_RPM_calculate(void)
{
	ROBOT_CHASSI.Motor_Target_RPM[0] = -ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_Target_RPM[1] = -ROBOT_CHASSI.SPEED.Robot_VY*COS45 - ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_Target_RPM[2] =  ROBOT_CHASSI.SPEED.Robot_VY*COS45 - ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
	ROBOT_CHASSI.Motor_Target_RPM[3] =  ROBOT_CHASSI.SPEED.Robot_VY*COS45 + ROBOT_CHASSI.SPEED.Robot_VX*COS45 + ROBOT_CHASSI.SPEED.World_W*MS_transition_RM;
}



void Robot_Wheels_Adjust(void)
{
	// unsigned char Flag;
	// Robot_Wheels_RPM_calculate();
	
	M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[0],ROBOT_CHASSI.Motor_Target_RPM[0],M3508_CHASSIS_MOTOR_REAL_INFO[0].RPM);
	M3508_CHASSIS_MOTOR_REAL_INFO[1].TARGET_CURRENT = PID_Speed_Calculate(&M3508_PID[1],ROBOT_CHASSI.Motor_Target_RPM[1],M3508_CHASSIS_MOTOR_REAL_INFO[1].RPM);

	M3508_Send_Currents(); //发送can信息使电机转起来
	//vTaskDelay(5);  
	
}

