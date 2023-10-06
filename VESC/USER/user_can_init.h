#ifndef __USER_CAN_INIT_H
#define __USER_CAN_INIT_H
#include "main.h"

typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_M3508_M1_ID = 0x201,
    CAN_M3508_M2_ID = 0x202,
    CAN_M3508_M3_ID = 0x203,
    CAN_M3508_M4_ID = 0x204,

}Can_Msg_Id_e;



typedef struct
{
    uint16_t ecd;            
    int16_t  speed_rpm;
    int16_t  given_current;
    uint8_t  temperate;
    int16_t  last_ecd;
} motor_measure_t;

void get_motor_measure(motor_measure_t *ptr, uint8_t data[8]);
void m3508_update_m3508_info(CAN_RxHeaderTypeDef *msg , uint8_t  Data[8]);
void User_Open_Can(void);
void M3508_Send_Currents_Reset(void);
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);
void Motor_PID_init (void);
void M3508_Send_Currents(int16_t Motor1,int16_t Motor2,int16_t Motor3,int16_t Motor4);
// M3508返回的电机真实信息


#endif
