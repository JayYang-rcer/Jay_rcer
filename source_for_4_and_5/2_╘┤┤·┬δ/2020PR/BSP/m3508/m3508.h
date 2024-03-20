#ifndef __M3508_H
#define __M3508_H
#include "stm32f4xx.h"

// M3508电机编号
#define M3508_CHASSIS_MOTOR_ID_1      0x201
#define M3508_CHASSIS_MOTOR_ID_2      0x202
#define M3508_CHASSIS_MOTOR_ID_3      0x203
#define M3508_CHASSIS_MOTOR_ID_4      0x204
#define M3508_ARM_MOTOR_ID            0x205
#define M2006_TURN_MOTOR_ID           0x206
#define GM6020_MOTOR_ID               0x20b


// M3508返回的电机真实信息
typedef struct M3508_REAL_INFO
{
	uint16_t ANGLE;   		        //采样角度						
	int16_t  RPM;								
	int16_t  CURRENT;
	int16_t  TARGET_CURRENT;
	  float  REAL_ANGLE;         //处理过的真实角度（必须用float）
}M3508_REAL_INFO;


extern struct M3508_REAL_INFO M3508_CHASSIS_MOTOR_REAL_INFO[4];   // 1-4分别对应顺时针方向的底盘电机
extern struct M3508_REAL_INFO M3508_ARM_MOTOR_REAL_INFO;          // 移动电机的参数结构体
extern struct M3508_REAL_INFO GM6020_TURN_MOTOR_REAL_INFO;         // 云台电机的参数结构体
extern struct M3508_REAL_INFO M2006_TURN_MOTOR_REAL_INFO;

extern struct PID M3508_ARM_MOTOR_PID_RPM;            // 夹球电机速度环PID
extern struct PID M3508_CHASSIS_MOTOR_PID_RPM[4];     // 驱动电机
extern struct PID GM6020_TURN_MOTOR_PID_RPM;          // GM6020电机速度环PID
extern struct PID M2006_TURN_MOTOR_PID_POS;           // M2006电机位置环PID
extern struct PID M2006_TURN_MOTOR_PID_RPM;           // 转盘电机速度环PID

extern int16_t ARM_TURN_MOTOR_TARGET_RPM ;    // 夹球电机目标速度
extern int16_t TURN_MOTOR_TARGET_RPM;    // 云台电机目标速度

void m3508_motor_init(void);
void m3508_update_m3508_info(CanRxMsg *msg);
void m2006_send_motor_currents(void);
void m3508_send_motor_currents(void);
void GM6020_send_motor_currents(void);
void m3508_ARM_angle_integral(M3508_REAL_INFO *M3508_MOTOR);
void m2006_TURN_MOTOR_angle_integral(M3508_REAL_INFO *M3508_MOTOR);

#endif




