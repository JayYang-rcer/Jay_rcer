#ifndef __MOVE__H
#define __MOVE__H
#include <stm32f4xx.h>

#define COS60 0.500000f
#define COS30 0.866025f
#define COS45 0.707106f
#define V_REAL    0.128f/60                      //轮子的线速度
#define  PI              3.1415926f    //PI的值
#define WHEEL_R              0.152f/2                 //轮子半径 
#define RM_transition_MS     (PI*WHEEL_R)/570.0f      //转速与速度的转换  
#define MS_transition_RM     570.0f/(PI*WHEEL_R)      //速度与转速的转换                 


extern int MOVE_OK ;

//机器人底盘数据结构体
typedef struct ROBOT_SPEED
{
 	float World_VX;
 	float World_VY;
 	float World_W;
 	float Robot_VX;
 	float Robot_VY;
} ROBOT_SPEED;


typedef struct ROBOT_CHASSIS
{
	ROBOT_SPEED SPEED;
  float Motor_RPM[4];           //4个轮子的转速
} ROBOT_CHASSIS;

//extern  ROBOT_CHASSIS  ROBOT_CHASSI;

//机器人路径跟踪时间管理
typedef struct TIMES
{
	uint16_t TIME_LAST;           //上次的时间
	uint16_t TIME_pass;           //经过的时间   
} TIMES;

typedef struct PATH_FOLLOW
{
	TIMES  Times;            //时间结构体
	uint8_t     COUNT;            //点数
} PATH_FOLLOW;


typedef struct ROBO_VELOCITY_PLANNING
{
	float up_a;         //加速的加速度
	float dn_a;
	float Vstart;       // 单位：RPM 绝对值
	float Vmax;
	float Vend;	
	float q1;           // 总路程
	float t;            // 时间t
	float t1;           //加速时间T1
	float t2;           //减速时间T2
}ROBO_VELOCITY_PLANNING;

extern ROBO_VELOCITY_PLANNING  v_planing;

void Robot_Wheels_RPM_calculate(void);
void Robot_Wheels_Adjust(void);
void Motor_PID_init (void);

#endif
