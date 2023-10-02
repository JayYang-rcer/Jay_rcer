#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#define START 0x11

enum FLAG{
	CHECK_HEADER_FLAG, 
	RECEIVE_DATA_FLAG, 
	CHECK_VALUE_FLAG, 
	FINAL_RECEIVE_FLAG
};


unsigned char serial_get_crc8_value(unsigned char *data, unsigned char len);
int STM32_READ_FROM_ROS(int *data1, int *data2, unsigned char *data3);
void USART_Send_String(unsigned char *p, short sendSize);
void Usart_Send_Data(short RPM_1, short RPM_2, float angle, char ctrlFlag);
int STM32_READ_FROM_ROS_FLOAT(int *data1, int *data2, float *target_angle, unsigned char *data3);

#endif
