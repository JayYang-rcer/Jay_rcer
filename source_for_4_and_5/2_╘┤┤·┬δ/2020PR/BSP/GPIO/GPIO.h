#ifndef __GPIO_H
#define __GPIO_H
#include <stm32f4xx.h>

// 气缸序号
#define CATCH_BALL_1ST        1
#define CATCH_SEAT_1          2
#define CATCH_SEAT_2          3
#define CATCH_SEAT_3          4
#define HOLD_FOOT             5
#define CATCH_BALL_SEAT       6

// 色标传感器序号
#define VERTICAL_LEFT_COLOR_SENSOR  1
#define ACLINIC_LEFT_COLOR_SENSOR   2
#define ACLINIC_RIGHT_COLOR_SENSOR  3
#define VERTICAL_RIGHT_COLOR_SENSOR 4

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


void cs_GPIO_init(void);
uint8_t Read_color(u8 sensor);

void beep_GPIO_init(void);

void cylinder_GPIO_init(void);
void cylinder_on(u8 cylinder);
void cylinder_off(u8 cylinder);

void ls_GPIO_init(void);
uint8_t Read_limit_switch(u8 sensor);

u16 Get_GPIO(void);
u8 GET_GPIO_KEY(void);
u16 Get_GPIO_limit(void);
void Key_GPIO_Init(void);












#endif
