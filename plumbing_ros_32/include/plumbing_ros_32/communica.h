#ifndef __COMMUNICA_H
#define __COMMUNICA_H

#include "ros/ros.h"
#include "boost/asio.hpp"


void SerialInit(void);
unsigned char serial_get_crc8_value(unsigned char *data, unsigned char len);
bool ROS_READ_FROM_STM32(int16_t &p_RPM1, int16_t &p_RPM2, unsigned char &crtlFlag);
void ROS_WRITE_TO_STM32(short target_RPM1, short target_RPM2, unsigned char crtlFlag);
bool ROS_READ_FROM_STM32_float(int16_t &p_RPM1, int16_t &p_RPM2, float &p_angle, unsigned char &crtlFlag);
void ROS_WRITE_TO_STM32_FLOAT(short target_RPM1, short target_RPM2, float angle, char crtlFlag);


#endif /* C9A0E3BD_B069_47C4_99B4_B59B1002337A */