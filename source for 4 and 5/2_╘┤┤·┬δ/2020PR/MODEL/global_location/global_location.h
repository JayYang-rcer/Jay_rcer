#ifndef __GLOBAL_LOCATION_H
#define __GLOBAL_LOCATION_H

#include "stm32f4xx.h"

// 选择使用哪款全场定位
#define USE_ACTION_GL	1
#define USE_SELF_GL	  0 

// 选择使用哪款全场定位
#define Action_Location  1  
#define Self_Location    0

// 初始时的位置偏置量
#define ROBOT_SZ_POS_OFFSET_X    (0) /*PARA*/
#define ROBOT_SZ_POS_OFFSET_Y    (0)

#define WHEEL_DIAMETER          50.8f  // 轮子直径
#define ENCODER_RESIOLUTION       1000  // 编码器分辨率
#define DISTANCE_PER_CNT (PI*WHEEL_DIAMETER/(ENCODER_RESIOLUTION*4))  // 每个CNT代表的路程值

#define Lx 173.18f  // 顺时针首轮子垂直底盘中心距离（单位：mm）
#define Ly 173.18f  // 顺时针尾轮子垂直底盘中心距离（单位：mm）

#define YAW0  45  // 顺时针首轮子离世界坐标系的角度（范围(-180, 180]）

// 机器人的真实位置
typedef struct ROBOT_REAL_POS
{
  float X_POS;
  float Y_POS;     
  float YAW_POS;
}ROBOT_REAL_POS;

// 东大全场定位模块定位的位置
typedef struct ACTION_GL_POS
{
	float ANGLE_Z;
	float ANGLE_X;
	float ANGLE_Y;	
	float POS_X;
	float POS_Y;
	float W_Z;
	
	float LAST_POS_X;
	float LAST_POS_Y;
	
	float DELTA_POS_X;
	float DELTA_POS_Y;	
}ACTION_GL_POS;


extern float OFFSET_YAW;
extern float SELF_GL_YAW_INIT_VALUE;
extern struct ROBOT_REAL_POS ROBOT_REAL_POS_DATA;

void Location_init(void);
void Update_Action_gl_position(float value[6]);
void Update_self_gl_position(void);

#endif

