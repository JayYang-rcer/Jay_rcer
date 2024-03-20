#ifndef __A80_H
#define __A80_H

#include "stdint.h"
#include "stm32f4xx.h"

typedef union Pos_limit
{
	uint8_t data[4];
	float ActVal;
}Pos_limit;


typedef union Encoder_Estimates
{
	uint8_t data[8];
	float ActVal[2];
}Encoder_Estimates;

typedef union IQ
{
	uint8_t data[8];
	float ActVal[2];
}IQ;


void Odrive_Control_pos1(CAN_TypeDef *CANx, u8 motor_num, int pos, float vel_limit, float accel_limit, float decel_limit);
void Odrive_Control_pos2(CAN_TypeDef *CANx, u8 motor_num, int pos, int16_t vel_FF, int16_t current_FF);
void Get_Encoder_Estimates_Tx(CAN_TypeDef *CANx, u8 drive_num);
void Get_Encoder_Estimates_Rx(CanRxMsg * msg);
void Get_IQ_Tx(CAN_TypeDef *CANx, u8 drive_num);
void Get_IQ_Rx(CanRxMsg * msg);
void odrive_control(u8 motor_num ,float t,float Target_t ,int motor_VEL);
void Odrive_Control_Vel(CAN_TypeDef *CANx, u8 motor_num, int vel);
void Odrive_Control_Cur(CAN_TypeDef *CANx, u8 motor_num, int cur);


extern Encoder_Estimates bldc_motor[13];
extern IQ bldc_motor_iq[13];
extern float T;

#endif
