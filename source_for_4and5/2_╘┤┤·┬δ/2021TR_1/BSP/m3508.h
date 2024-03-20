#ifndef __M3508_H
#define __M3508_H
#include "stm32f4xx.h"



// M3508返回的电机真实信息
typedef struct M3508_REAL_INFO
{
	uint16_t ANGLE;   		        //采样角度						
	int16_t  RPM;								
	int16_t  CURRENT;
	int16_t  TARGET_CURRENT;
	
	// 角度积分时用到下面变量
	float		 REAL_ANGLE;         //处理过的真实角度（必须用float）
	u8			 FIRST_ANGLE_INTEGRAL_FLAG;
	uint16_t LAST_ANGLE;
}M3508_REAL_INFO;


extern struct M3508_REAL_INFO M3508_CHASSIS_MOTOR_REAL_INFO[4];   // 1-4分别对应顺时针方向的底盘电机
extern struct M3508_REAL_INFO M3508_CAST_MOTOR_REAL_INFO;
extern struct M3508_REAL_INFO M3508_PITCH_MOTOR_REAL_INFO;
extern struct M3508_REAL_INFO M2006_YAW_MOTOR_REAL_INFO;

extern struct PID M3508_CHASSIS_MOTOR_PID_RPM[4];     // 驱动电机
extern struct PID M3508_CAST_MOTOR_PID_RPM;
extern struct PID M3508_PITCH_MOTOR_PID_RPM;
extern struct PID M2006_YAW_MOTOR_PID_RPM;

void M3508_Motor_Init(void);
void m3508_update_m3508_info(CanRxMsg *msg);
void chassis_m3508_send_motor_currents(void);
void shoot_m3508_send_motor_currents(void);
void M3508AngleIntegral(M3508_REAL_INFO *M3508_MOTOR);

#endif




