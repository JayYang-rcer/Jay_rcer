#ifndef __CYLINDER_H
#define __CYLINDER_H

#include "stm32f4xx.h"

// Æø¸×ÐòºÅ
#define HOLD_FOOT             1
#define SHOOT_PULL            2
#define CATCH_BALL_2ST        3
#define TURN_CATCH            4
#define CATCH_BALL_1ST        5
#define CATCH_BALL_SEAT       6


void cylinder_GPIO_init(void);
void cylinder_on(u8 cylinder);
void cylinder_off(u8 cylinder);

#endif 
