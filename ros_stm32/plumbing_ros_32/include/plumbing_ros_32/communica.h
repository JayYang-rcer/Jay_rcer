#ifndef __COMMUNICA_H
#define __COMMUNICA_H

#include "ros/ros.h"
#include "boost/asio.hpp"


void SerialInit(void);
unsigned char serial_get_crc8_value(unsigned char *data, unsigned char len);
bool ROS_READ_FROM_STM32(float &p_robot_x, float &p_robot_y, float &p_robot_yaw, unsigned char &crtlFlag);
void ROS_WRITE_TO_STM32(float chassis_x, float chassis_y, float chassis_w, unsigned char chassis_control_flag, 
                        unsigned char BP_ctrl_state, unsigned char FW_ctrl_state);


#endif /* C9A0E3BD_B069_47C4_99B4_B59B1002337A */