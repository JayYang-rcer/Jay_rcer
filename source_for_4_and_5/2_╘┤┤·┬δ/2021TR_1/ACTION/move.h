#ifndef __MOVE_H
#define __MOVE_H

#include "global_location.h"
#include "pid.h"

typedef struct
{
	float X;
	float Y;
	float Yaw;
	float V_x;
	float V_y;
	float W;
}PATH_TYPEDEF;


typedef enum
{
	MOVE_STOP,
	
	// 射箭点
	MOVE_2_CAST_POINT_1,
	MOVE_2_CAST_POINT_2,
	
	// 取箭点
	MOVE_2_LOAD_ARROW,
	
	// 启动区
	MOVE_2_RESTART
	
}MOVE_STATE_ITEMS;



void MoveInit(void);
void YawAdjust(float Target_angle);
void PDController(PATH_TYPEDEF target_point, ROBOT_REAL_POS robot_now_pos);
int PathPlan(float t_real, float t_target, int num, float *X, float *Y, float *Yaw);
void LockupPoint(float POS_X, float POS_Y, float POS_YAW);


extern PID yaw_pid;
extern PID point_X_pid;
extern PID point_Y_pid;

extern MOVE_STATE_ITEMS MOVE_STATE;

#endif
