#ifndef __M3508_DRIVE_H
#define __M3508_DRIVE_H
#include "stm32f4xx.h"

#define TURNPLATE_INIT_POS            0     // 转盘初始位置
#define TURNPLATE_1st_SHOOT_POS    -32.5    // 转盘射第一个球位置//32.5
#define TURNPLATE_2nd_SHOOT_POS    -41.5    // 转盘射第二个球位置
#define TURNPLATE_3rd_SHOOT_POS    -50.4    // 转盘射第三个球位置
#define TURNPLATE_4th_SHOOT_POS    -37.2    // 转盘射第四个球位置
#define TURNPLATE_5th_SHOOT_POS    -32.1    // 转盘射第五个球位置

//#define TURNPLATE_INIT_POS            0     // 转盘初始位置
//#define TURNPLATE_1st_SHOOT_POS    -29.8    // 转盘射第一个球位置
//#define TURNPLATE_2nd_SHOOT_POS    -48.5    // 转盘射第二个球位置
//#define TURNPLATE_3rd_SHOOT_POS    -55.5    // 转盘射第三个球位置
//#define TURNPLATE_4th_SHOOT_POS    -37.0    // 转盘射第四个球位置
//#define TURNPLATE_5th_SHOOT_POS    -28.3    // 转盘射第五个球位置

#define ARM_INIT_POS                  0    // 夹球机械臂的初始位置
#define ARM_SHOOT_POS              -90    // 放下夹球要求到达的位置

	


typedef struct ARM_VELOCITY_PLANNING //夹球机构的速度规划
{
	float Pstart;        //开始位置
	float Pend;          //结束位置
	float Vstart;        //开始的速度           // 单位：RPM 绝对值
	float Vmax;          //最大的速度
	float Vend;          //末尾的速度
	float Rac;           //加速路程的比例
	float Rde;           //减速路程的比例
}ARM_VELOCITY_PLANNING;

typedef struct TURNPLATE_VELOCITY_PLANNING  //发射转盘机构的速度规划
{
	float Pstart;        //开始位置
	float Pend;          //结束位置
	float Vstart;        //开始的速度           // 单位：RPM 绝对值
	float Vmax;          //最大的速度
	float Vend;          //末尾的速度
	float Rac;           //加速路程的比例
	float Rde;           //减速路程的比例
}TURNPLATE_VELOCITY_PLANNING;

extern struct ARM_VELOCITY_PLANNING *ARM_NOW_MOTION; 
extern struct ARM_VELOCITY_PLANNING ARM_HOLD_MOTION;  
extern struct ARM_VELOCITY_PLANNING ARM_RESET_MOTION;
extern struct ARM_VELOCITY_PLANNING ARM_SHOOT_MOTION;
extern struct ARM_VELOCITY_PLANNING ARM_SHOOT_MOTION2;
extern struct ARM_VELOCITY_PLANNING ARM_SHOOT_2_MOTION;



void Motor_motion_control(void *pdata);                                                     //电机动作控制
void M3508_arm_motor_motion_control(void);                                                  //机械臂机构任务管理
void M2006_turnplate_motion_control(void);                                                  //转盘动作控制

void ad_plan_arm_motor_RPM(ARM_VELOCITY_PLANNING motion, float pos);                        //机械臂机构速度规划



#endif
