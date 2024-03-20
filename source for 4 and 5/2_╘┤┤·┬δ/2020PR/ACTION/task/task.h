#ifndef __TASK_H
#define __TASK_H

#include "stm32f4xx.h"

#define ANGLE_2P      -122
#define ANGLE_3P      -115


void pass_ball_task(void *pdata);
void KICK_control(void);
void shoot_ball_task(void);
void ARM_RESET(void);	
void romote_action_task(void);
void TURNPLATE_adjusting(float Target_angle);
void TURNPLATE_adjust(float P_start, float P_end, float V_start, float V_max ,float V_end,float Rac ,float Rde ,float pos);
int DELAY_ms(int time_delay);
#endif
