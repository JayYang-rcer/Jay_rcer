#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f4xx.h"

#define BEEP_TIME 1000 // 蜂鸣器响的时间（单位：ms）

void beep_GPIO_init(void);
void beep_control(void);
void beep_turn_on(void);
void beep_turn_off(void);

#endif
