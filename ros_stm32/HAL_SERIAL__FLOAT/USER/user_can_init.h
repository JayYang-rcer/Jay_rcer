#ifndef __USER_CAN_INIT_H
#define __USER_CAN_INIT_H
#include "main.h"

typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CHASSIS_M3508_M1_ID = 0x201,
    CHASSIS_M3508_M2_ID = 0x202,
    CHASSIS_M3508_M3_ID = 0x203,
    CHASSIS_M3508_M4_ID = 0x204,

}Can_Msg_Id_e;



typedef struct
{
    int16_t ANGLE;            	// 转子机械角度
    int16_t  RPM;				// 实际转子转速
    int16_t  CURRENT;			// 实际转矩电流
    int  TARGET_CURRENT;	// 目标转矩电流
	float  REAL_ANGLE;
}M3508_REAL_INFO;

void get_motor_measure(CAN_RxHeaderTypeDef *msg, uint8_t Data[8]);
void User_Open_Can(void);
void M3508_Send_Currents_Reset(void);
void Motor_PID_init (void);
void M3508_Send_Currents(void);
// M3508返回的电机真实信息

extern M3508_REAL_INFO M3508_CHASSIS_MOTOR_REAL_INFO[4]; 

#endif
