#ifndef __GYRO_H
#define __GYRO_H

#include "stm32f4xx.h"
#include "string.h"

void gyro_UART4_init(u32 baud_rate);
void gyro_processing_gyro_data(unsigned char ucData);

struct SAngle
{
	short Angle[3];
	short T;
};

extern struct SAngle GYRO_ANGLE;

#endif
