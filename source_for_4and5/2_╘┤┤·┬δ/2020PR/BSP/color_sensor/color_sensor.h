#ifndef __COLOR_SENSOR_H
#define __COLOR_SENSOR_H

#include "stm32f4xx.h"

// 色标传感器序号
#define VERTICAL_LEFT_COLOR_SENSOR  1
#define ACLINIC_LEFT_COLOR_SENSOR   2
#define ACLINIC_RIGHT_COLOR_SENSOR  3
#define VERTICAL_RIGHT_COLOR_SENSOR 4

void cs_GPIO_init(void);
uint8_t Read_color(u8 sensor);

#endif
