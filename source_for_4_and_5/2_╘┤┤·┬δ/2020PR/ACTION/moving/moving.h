#ifndef __MOVING_H
#define __MOVING_H

#include "stm32f4xx.h"


// 调整底盘速度函数的运行周期
#define TRACK_RUN_PERIOD 1  // 单位：ms

// 是否开启PID衰减功能
#define IF_OPEN_PID_ATTENUATION_ARC_PID 0
#define IF_OPEN_PID_ATTENUATION_LINE_PID 0
#define IF_OPEN_PID_ATTENUATION_POINT_PID 0

// 是否开启限制总速度的功能
#define IF_LIMIT_TOTAL_VELOCITY_ARC_TRACK 1
#define IF_LIMIT_TOTAL_VELOCITY_LINE_TRACK 1

typedef struct robot_struct
{
	float x;
	float y;
	float yaw;
	float angle;
}robot_struct;

typedef struct yaw_struct
{
	float yaw;
}yaw_struct;

//机器人路径跟踪时间管理
typedef struct TIMES
{
	double TIME_LAST;           //上次的时间
	double TIME_pass;           //经过的时间   
} TIMES;

typedef struct PATH_FOLLOW
{
	TIMES  Times;            //时间结构体
	int     COUNT;            //点数
} PATH_FOLLOW;


//点的结构体 单位mm
typedef struct Point_t
{
	float x;
	float y;
}Point_t;


typedef struct Pose_t
{
	Point_t point;
	//角度制
	float  direction;
	//速度
	float vel;
	//时间
	float T;
	
}Pose_t;

extern struct ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;
extern struct Pose_t *NOW_PATH;
extern struct PID point_pid;
extern struct PID yaw_pid;
extern struct robot_struct robot;
extern float motion_t;

void moving_task(void *pdata);
void moving_init(void);

void point_follow(float X_start,float Y_start, float X_end, float Y_end,float V_start, float V_max ,float V_end,float Rac ,float Rde ,float pos_x,float pos_y);
void PATH_TRACKING(Pose_t *Path , int PATH_POINT_SUM);
void Pathplan(float t,float Target_t,int num ,float *X ,float *Y );
void moving_yaw_adjusting(float Target_angle);
void moving_point_track(float X_POS,float Y_POS);
/**
* @brief  AngleLimit角度限幅，将角度限制在-180°到180°
  * @note
* @param  angle:要限制的值
* @retval 
  */
void AngleLimit(float *angle);

//纠正角度
float Angle_adjust(float Sx_error,float Sy_error);

#endif
