#ifndef __TIME_H
#define __TIME_H

#include "stm32f4xx.h"

void encoder_TIM4_init(void);
void encoder_TIM8_init(void);
void Launch_Motor_init(u32 arr,u32 psc);
u32 encoder_get_encoder_CNT(u8 TIMx);
void TIM9_PWM_Init(u32 arr,u32 psc);
#endif
