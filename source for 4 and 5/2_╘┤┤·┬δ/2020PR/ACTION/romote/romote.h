#ifndef __ROMOTE_H
#define __ROMOTE_H

#include "stm32f4xx.h"

// 遥控控制速度（正值）
#define ROMOTE_SZ_Vx 1000
#define ROMOTE_SZ_Vy 400 /*PARA*/
#define ROMOTE_SZ_W  400

#define ROMOTE_TZ_Vx 400
#define ROMOTE_TZ_Vy 400 /*PARA*/
#define ROMOTE_TZ_W  300

#define ROMOTE_FREE_Vx 1500
#define ROMOTE_FREE_Vy 1500 /*PARA*/
#define ROMOTE_FREE_W  800

#define RELAY_Vx 300
#define RELAY_Vy 300

void romote_action_task(void);
void romote_choose_mode(void);
void romote_control_moving_for_grab(void);
void romote_control_moving_for_throw(void);
void romote_control_relay(void);

#endif

