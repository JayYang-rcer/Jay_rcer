#ifndef __LIMIT_SWITCH_H
#define __LIMIT_SWITCH_H

#include "stm32f4xx.h"

// 限位开关序号
#define UNDER_ARM_RESET_LIMIT_SWITCH              1
#define TURNPLATE_RESET_LIMIT_SWITCH              2
#define ARM_ARRIVE_LIMIT_SWITCH                   3
#define PULL_RESET_LIMIT_SWITCH                   4
#define CHASSIS_RIGHT_LIMIT_SWITCH                5
#define CHASSIS_LEFT_LIMIT_SWITCH                 6
#define ARM_RESET_LIMIT_SWITCH                    7
#define MOVE_ARM_RESET_LIMIT_SWITCH               8
#define MOVE_ARM_MIDDLE_LIMIT_SWITCH              9
#define LIGHT_SWITCH_1                           10
#define LIGHT_SWITCH_2                           11

#define LIMITED_V 0 /*PARA*/

void ls_GPIO_init(void);
uint8_t Read_limit_switch(u8 sensor);
void ls_use_limit_switch_limit_velocity(void);

#endif
