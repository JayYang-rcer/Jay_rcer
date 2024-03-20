#ifndef __CAST_H
#define __CAST_H

#include "stm32f4xx.h"

#define CAST_V_MAX	1.5f
#define CAST_V_MIN	0.3f
#define CAST_ACC		0.01f

typedef struct MOTOR_VELOCITY_PLANNING
{
	float Pstart;        //开始位置
	float Pend;          //结束位置
	float Vstart;        //开始的速度           // 单位：RPM 绝对值
	float Vmax;          //最大的速度
	float Vend;          //末尾的速度
	float Rac;           //加速路程的比例
	float Rde;           //减速路程的比例
}MOTOR_VELOCITY_PLANNING;


int MotorRPMPlan(MOTOR_VELOCITY_PLANNING motion, float pos, int16_t *motor_target_rpm);

extern float CAST_MOTOR_TARGET_RPM;

extern int TopSwitch_State;
extern float cast_target_v;

#endif
