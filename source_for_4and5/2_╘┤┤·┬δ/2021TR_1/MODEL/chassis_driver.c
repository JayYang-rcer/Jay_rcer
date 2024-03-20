#include "includes.h"

ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;
CHASSIS_MOTOR_RPM CHASSIS_MOTOR_TARGET_RPM;

// 4轮机器人坐标系逆运动学
// 世界坐标系
// W：正值-逆时针 负值-顺时针
void Robot_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM)
{
	CHASSIS_MOTOR_TARGET_RPM.MOTOR1_RPM = ( COS45 * Vy_RPM + SIN45 * Vx_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR2_RPM = ( COS45 * Vy_RPM - SIN45 * Vx_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR3_RPM = (-COS45 * Vy_RPM - SIN45 * Vx_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR4_RPM = (-COS45 * Vy_RPM + SIN45 * Vx_RPM + Robot_R*W_RPM) * MS_TO_RPM;
}


// 4轮世界坐标系逆运动学
// theta为机器人坐标系x轴与世界坐标系x轴夹角 单位：度
// W：正值-逆时针 负值-顺时针
void World_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM, float theta)
{
	theta = PI * theta / 180.0f;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR1_RPM = ( cos(theta-PI/4.0f) * Vy_RPM - sin(theta-PI/4.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR2_RPM = ( cos(theta+PI/4.0f) * Vy_RPM - sin(theta+PI/4.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR3_RPM = (-cos(theta-PI/4.0f) * Vy_RPM + sin(theta-PI/4.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR4_RPM = (-cos(theta+PI/4.0f) * Vy_RPM + sin(theta+PI/4.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
}


// 3轮世界坐标系逆运动学
// theta为机器人坐标系x轴与世界坐标系x轴夹角 单位：度
// W：正值-逆时针 负值-顺时针 
void World_3wheels(float Vx_RPM, float Vy_RPM, float W_RPM, float theta)
{
	theta = PI * theta / 180.0f;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR1_RPM = ( cos(theta) * Vy_RPM - sin(theta) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR2_RPM = (-cos(theta+PI/3.0f) * Vy_RPM - sin(theta+PI/3.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_TARGET_RPM.MOTOR3_RPM = (-sin(theta+PI/6.0f) * Vy_RPM + cos(theta+PI/6.0f) * Vx_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
}


