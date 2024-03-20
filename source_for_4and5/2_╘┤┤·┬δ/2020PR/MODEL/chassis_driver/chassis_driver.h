#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "stm32f4xx.h"

#define COS45              0.70710678f
#define SIN45              0.70710678f

#define Robot_R            0.55f                  //车轮到中心距离(单位：m)

#define V_REAL             0.128f/60                //轮子的线速度

#define WHEEL_R            0.152f/2                 //轮子半径(单位：m) 
#define M3508_RM_To_MS     (PI*WHEEL_R)/570.0f      //转速与速度的转换 (单位：m/s) 
#define M3508_MS_To_RM     570.0f/(PI*WHEEL_R)      //速度与转速的转换 (单位：m/s)  

#define RM3508             1
#define RE40               0
// 计算公式：1/（pi*轮子直径）*减速比*60
#define MS_TO_RPM          21*60/(PI*WHEEL_R*2)     //轮子直径152mm，电机减速比1:21，轮子一圈pi*152mm

// 底盘电机RPM
typedef struct CHASSIS_MOTOR_RPM
{
	float MOTOR1_RPM;
	float MOTOR2_RPM;
	float MOTOR3_RPM;
	float MOTOR4_RPM;
}CHASSIS_MOTOR_RPM;

// 底盘期望速度
typedef struct ROBOT_TARGET_VELOCITY
{
	float Vx;
	float Vy;
	float W;
	float Vx_RPM;
	float Vy_RPM;
	float W_RPM;
}ROBOT_TARGET_VELOCITY;

extern struct ROBOT_REAL_POS ROBOT_REAL_POS_DATA;

void cd_chassis_driver_init(void);
void cd_robot_kinematic_conversion_3wheels(float Vx, float Vy, float Vz);
void cd_world_kinematic_conversion_3wheels(float Vx, float Vy, float W, float theta);
void Robot_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM);
void World_4wheels(float Vx, float Vy, float W, float theta);
void cd_velocity_adjust(void *pdata);

#endif
