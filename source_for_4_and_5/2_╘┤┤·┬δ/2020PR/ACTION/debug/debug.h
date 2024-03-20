#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f4xx.h"

#define LOCATION_DEBUG             0
#define VELOCITY_DEBUG             0
#define M3508_DEBUG                0
#define DEBUG_PATH_TRACKING        0
#define DEBUG_COLOR_SENSOR         0
#define DEBUG_LASER_RANGING        0
#define DEBUG_LIMIT_SWITCH         0
#define DEBUG_PS2                  0



void Debug_by_UART(void *pdata);


#endif
