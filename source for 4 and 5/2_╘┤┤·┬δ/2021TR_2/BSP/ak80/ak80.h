#ifndef __AK80_H
#define __AK80_H

#include "sys.h"
#include "pid.h"
#include "defines.h"

#define CMD_MOTOR_MODE      0x01
#define CMD_RESET_MODE      0x02
#define CMD_ZERO_POSITION   0x03

#define AK80_YAW_ID       	0x01
#define AK80_PITCH_ID       0x02
#define MIT_DRIVER_ID_1			0x03
#define MIT_DRIVER_ID_2			0x04

//根据手册限幅
#define P_MIN               -95.5f
#define P_MAX                95.5f
#define V_MIN               -30.0f
#define V_MAX 							 30.0f
#define T_MIN 							-18.0f
#define T_MAX 							 18.0f
#define Kp_MIN 								0
#define Kp_MAX 							 500.0f
#define Kd_MIN 							  0
#define Kd_MAX   						  5.0f
#define I_MAX  							 18.0f

#define MIT_P_MIN						-12.5f
#define MIT_P_MAX						 12.5f
#define MIT_V_MIN						-500.0f
#define MIT_V_MAX						 500.0f

// M3508返回的电机真实信息
typedef struct AK80_REAL_INFO
{
	float ANGLE;   		        //采样角度	(rad)			
	float  V_angle;						  //采样角速度(rad/s	)	
	float  CURRENT;
	float  TARGET_CURRENT;
	float  TARGET_POS;
	float  TARGET_SPEED;
	float  REAL_ANGLE;         //处理过的真实角度（必须用float）
}AK80_REAL_INFO;


void Mit_Driver_Init(void);
float fmaxf(float a, float b);
float fminf(float a, float b);
int float_to_uint(float x, float x_min, float x_max, int bits) ;
void AK80_motion_control(u8 ID,float p_des, float v_des, float kp, float kd, float t_ff);
void ak80_control_cmd(uint8_t ID,uint8_t cmd);
void MIT_Driver_Current_Send(u8 ID, float t_ff);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void AK80_update_info(CanRxMsg *msg);
extern struct AK80_REAL_INFO MIT_DRIVER_REAL_INFO[2];
extern PID MIT_DRIVER_PID_INFO[2];

#endif
