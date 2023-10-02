#ifndef  __CHASSIS_H
#define  __CHASSIS_H

void Speed_Calculate(void);
void Robot_Wheels_Adjust(void);

#define COS60 0.500000f
#define COS30 0.866025f
#define COS45 0.707106f
#define V_REAL    0.128f/60                      //轮子的线速度
#define  PI              3.1415926f    //PI的值
#define WHEEL_R              0.152f/2                 //轮子半径 
#define RM_transition_MS     (PI*WHEEL_R)/570.0f      //转速与速度的转换  
#define MS_transition_RM     570.0f/(PI*WHEEL_R)      //速度与转速的转换    


//机器人底盘数据结构体
typedef struct ROBOT_SPEED
{
 	int World_W;
 	int Robot_VX;
 	int Robot_VY;
} ROBOT_SPEED;


typedef struct ROBOT_CHASSIS
{
	ROBOT_SPEED SPEED;
	float Motor_Target_RPM[4];           //4个轮子的目标转速
} ROBOT_CHASSIS;

extern ROBOT_CHASSIS ROBOT_CHASSI;

#endif

