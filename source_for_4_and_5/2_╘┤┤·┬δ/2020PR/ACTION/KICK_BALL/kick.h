#ifndef __KICK_H
#define __KICK_H
#include "stm32f4xx.h"


#define RESET_KICK_DELAY    100     // 重置推球气缸的延时长度
#define WAIT_KICK_DELAY     400     // 摇臂夹球延时长度
#define KICK_BALL_DELAY     100     // 踢球回溯延时长度
#define PREPARE_KICK_DELAY  100     // 准备抬起脚延时长度

void kick_action_task(void);

#endif

