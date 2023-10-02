#include "main.h"

//通信协议常量
const unsigned char serial_header[2]  = {0x55, 0xaa};
const unsigned char serial_ender[2]   = {0x0d, 0x0a};

//左右轮速控制速度共用体
union Recieve_Data_RPM
{
	short data;
	unsigned char tem_array[2];
}TARGET_RPM1,TARGET_RPM2;

union Recieve_Data_ANGLE
{
	float data;
	unsigned char tem_array[4];
}TARGET_ANGLE;

//发送数据
union Send_Data_RPM
{
    short data;
    unsigned char tem_array[2];
}NOW_RMP_1,NOW_RMP_2;

union Send_Data_Angle
{
	float data;
	unsigned char tem_array[4];
}REAL_ANGLE;




int testRece1 = 0x00;
int testRece2 = 0x00;
unsigned char testRece4 = 0x00;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        STM32_READ_FROM_ROS_FLOAT(&testRece1,&testRece2,&ROBOT_CHASSI.Motor_Target_RPM[1],&testRece4);
		//printf("%data\r\n",M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT);
    }
}


// unsigned char receive_send_buf[920] = {0};
// unsigned char check_header_send_buf[2] = {0};
// unsigned char USART_Receiver1 = 0x00;
// int STM32_READ_FROM_ROS(int *data1, int *data2, unsigned char *data3)
// {
// 	static short index = 0;
// 	static unsigned char flag = CHECK_HEADER_FLAG;

// 	//接收串口数据
// 	HAL_UART_Receive_IT(&huart1, &USART_Receiver1,1);

// 	switch (flag)
// 	{
// 		case CHECK_HEADER_FLAG:
// 		{
// 			for (int i = 0; i < 1; i++)
// 			{
// 				check_header_send_buf[i] = check_header_send_buf[i+1];
// 			}
// 			check_header_send_buf[1] = USART_Receiver1;
			
// 			unsigned char count = 0;

// 			for (int i = 0; i < 2; i++)
// 			{
// 				if (check_header_send_buf[i] == serial_header[i])
// 				{
// 					count++;
// 				}
// 			}

// 			if (count == 2)
// 			{
// 				while (index < 2)
// 				{
// 					receive_send_buf[index] = check_header_send_buf[index];
// 					index++;
// 				}
// 				flag = RECEIVE_DATA_FLAG;
// 			}
			
// 			break;
// 		}

// 		case RECEIVE_DATA_FLAG:
// 		{
// 			receive_send_buf[index] = USART_Receiver1;
// 			index++;
			
// 			if(index == 8)
// 			{
// 				flag = CHECK_VALUE_FLAG;
// 			}
// 			break;
// 		}
// 		case CHECK_VALUE_FLAG:
// 		{
// 			unsigned char check_value = serial_get_crc8_value(receive_send_buf, index - 1);
// 			if(check_value == USART_Receiver1)
// 			{
// 				receive_send_buf[index] = USART_Receiver1;
// 				index++;
// 				flag = FINAL_RECEIVE_FLAG;
// 			}
// 			else
// 			{
// 				index = 0;
// 				flag = CHECK_HEADER_FLAG;
// 			}
// 			break;
// 		}

// 		case FINAL_RECEIVE_FLAG:
// 		{
// 			receive_send_buf[index] = USART_Receiver1;
// 			index++;

// 			if (index == 11)
// 			{
// 				for(int i = 0; i < 2; i++)
// 				{
// 					TARGET_RPM1.tem_array[i] = receive_send_buf[i + 3];
// 				}

// 				for(int i = 0; i < 2; i++)
// 				{
// 					TARGET_RPM2.tem_array[i] = receive_send_buf[i + 5];
// 				}

// 				*data1 = TARGET_RPM1.data;
// 				*data2 = TARGET_RPM2.data;
// 				*data3 = receive_send_buf[7];
				
// 				index = 0;
// 				flag = CHECK_HEADER_FLAG;
// 				return 1;
// 			}

// 			break;
// 		}
		
// 		default: break;
// 	}

// 	return 0;
// }

//数据接收暂存区
unsigned char receivesend_buff[16]={0};
unsigned char USART_Receiver1              = 0;          //接收数据
int STM32_READ_FROM_ROS(int *data1, int *data2, unsigned char *data3)
{
	static unsigned char checkSum             = 0;
	static unsigned char USARTsend_bufferIndex     = 0;
	static short j=0,k=0;
	static unsigned char USARTReceiverFront   = 0;
	static unsigned char Start_Flag           = START;      //一帧数据传送开始标志位
	static short dataLength                   = 0;

	HAL_UART_Receive_IT(&huart1,&USART_Receiver1,1);

	if(Start_Flag == START)
	{
		if(USART_Receiver1 == 0xaa)                             //buf[1]
		{  
			if(USARTReceiverFront == 0x55)         //数据头两位 //buf[0]
			{
				Start_Flag = !START;              //收到数据头，开始接收数据
				//printf("header ok\n");
				receivesend_buff[0]=serial_header[0];         //buf[0]
				receivesend_buff[1]=serial_header[1];         //buf[1]
				USARTsend_bufferIndex = 0;             //缓冲区初始化
				checkSum = 0x00;				  //校验和初始化
			}
		}
		else 
		{
			USARTReceiverFront = USART_Receiver1;  
		}
	}
	else
    { 
		switch(USARTsend_bufferIndex)
		{
			case 0://接收数据的长度
				receivesend_buff[2] = USART_Receiver1;
				dataLength     = receivesend_buff[2];            //buf[2]
				USARTsend_bufferIndex++;
				break;
			case 1://接收所有数据，并赋值处理 
				receivesend_buff[j + 3] = USART_Receiver1;        //buf[3] buf[4]  /buf[5] buf[6]	/buf[7]			
				j++;
				if(j >= dataLength)                         
				{
					j = 0;
					USARTsend_bufferIndex++;
				}
				break;
			case 2://接收校验值信息(设定为0x07)
				receivesend_buff[3 + dataLength] = USART_Receiver1;
				checkSum = serial_get_crc8_value(receivesend_buff, 3 + dataLength); //buf[8]
//				checkSum = 0x07;
				  // 检查信息校验值
				if (checkSum != receivesend_buff[3 + dataLength]) //buf[8]
				{
					printf("Received tem_array check sum error!\r\n");
					return 0;
				}
				USARTsend_bufferIndex++;
				break;
				
				case 3://接收信息尾
				if(k==0)
				{
					//数据0d     buf[9]  无需判断
					k++;
				}
				else if (k==1)
				{
					//数据0a     buf[10] 无需判断

					//进行速度赋值操作					
					 for(k = 0; k < 2; k++)
					{
						TARGET_RPM1.tem_array[k]  = receivesend_buff[k + 3]; //buf[3]  buf[4] 
						TARGET_RPM2.tem_array[k] = receivesend_buff[k + 5]; //buf[5]  buf[6]
					}				
					//赋值操作
					*data3 = receivesend_buff[7];
					*data1 = TARGET_RPM1.data;//偏航角
					*data2 = TARGET_RPM2.data;//俯仰角
					//-----------------------------------------------------------------
					//完成一个数据包的接收，相关变量清零，等待下一字节数据
					USARTsend_bufferIndex   = 0;
					USARTReceiverFront = 0;
					Start_Flag         = START;
					checkSum           = 0;
					dataLength         = 0;
					j = 0;
					k = 0;
					//-----------------------------------------------------------------					
				}
				break;
			 default:break;
		}
	}
	return 0;
}


int STM32_READ_FROM_ROS_FLOAT(int *data1, int *data2, float *target_angle, unsigned char *data3)
{
	static unsigned char checkSum             = 0;
	static unsigned char USARTsend_bufferIndex     = 0;
	static short j=0,k=0;
	static unsigned char USARTReceiverFront   = 0;
	static unsigned char Start_Flag           = START;      //一帧数据传送开始标志位
	static short dataLength                   = 0;

	HAL_UART_Receive_IT(&huart1,&USART_Receiver1,1);

	if(Start_Flag == START)
	{
		if(USART_Receiver1 == 0xaa)                             //buf[1]
		{  
			if(USARTReceiverFront == 0x55)         //数据头两位 //buf[0]
			{
				Start_Flag = !START;              //收到数据头，开始接收数据
				//printf("header ok\n");
				receivesend_buff[0]=serial_header[0];         //buf[0]
				receivesend_buff[1]=serial_header[1];         //buf[1]
				USARTsend_bufferIndex = 0;             //缓冲区初始化
				checkSum = 0x00;				  //校验和初始化
			}
		}
		else 
		{
			USARTReceiverFront = USART_Receiver1;  
		}
	}
	else
    { 
		switch(USARTsend_bufferIndex)
		{
			case 0://接收数据的长度
				receivesend_buff[2] = USART_Receiver1;
				dataLength     = receivesend_buff[2];            //buf[2]
				USARTsend_bufferIndex++;
				break;
			case 1://接收所有数据，并赋值处理 
				receivesend_buff[j + 3] = USART_Receiver1;        //buf[3] ~ buf[11]	
				j++;
				if(j >= dataLength)    //进入条件 j = 9                     
				{
					j = 0;
					USARTsend_bufferIndex++;
				}
				break;
			case 2://接收校验值信息(设定为0x07)
				receivesend_buff[3 + dataLength] = USART_Receiver1;
				checkSum = serial_get_crc8_value(receivesend_buff, 3 + dataLength); //buf[8]
//				checkSum = 0x07;
				  // 检查信息校验值
				if (checkSum != receivesend_buff[3 + dataLength]) //buf[8]
				{
					printf("Received tem_array check sum error!\r\n");
					return 0;
				}
				USARTsend_bufferIndex++;
				break;
				
				case 3://接收信息尾
				if(k==0)
				{
					//数据0d     buf[9]  无需判断
					k++;
				}
				else if (k==1)
				{
					//数据0a     buf[10] 无需判断

					//进行速度赋值操作					
					 for(k = 0; k < 2; k++)
					{
						TARGET_RPM1.tem_array[k]  = receivesend_buff[k + 3]; //buf[3]  buf[4] 
						TARGET_RPM2.tem_array[k] = receivesend_buff[k + 5]; //buf[5]  buf[6]
					}		
					
					for(k = 0; k < 4; k++)
					{
						TARGET_ANGLE.tem_array[k] = receivesend_buff[k + 7]; //buf[7] buf[8] buf[9] buf[10]
					}
							
					//赋值操作
					*data3 = receivesend_buff[11];	//buf[11]
					*data1 = TARGET_RPM1.data;//电机1转速
					*data2 = TARGET_RPM2.data;//电机2转速
					*target_angle = TARGET_ANGLE.data;
					//-----------------------------------------------------------------
					//完成一个数据包的接收，相关变量清零，等待下一字节数据
					USARTsend_bufferIndex   = 0;
					USARTReceiverFront = 0;
					Start_Flag         = START;
					checkSum           = 0;
					dataLength         = 0;
					j = 0;
					k = 0;
					//-----------------------------------------------------------------					
				}
				break;
			 default:break;
		}
	}
	return 0;
}



void Usart_Send_Data(short RPM_1, short RPM_2, float angle, char ctrlFlag)
{
    // 协议数据缓存数组
	unsigned char send_buf[15] = {0};
	int i, Length = 0;

    //赋值两个电机的转子转速
    NOW_RMP_1.data = RPM_1;
    NOW_RMP_2.data = RPM_2;
	REAL_ANGLE.data = angle;

    //设置消息头
    for (i = 0; i < 2; i++)
    {
        send_buf[i] = serial_header[i];
    }

    Length = 9; //2 + 2 + 1 + 4 = 9
	send_buf[2] = Length;    
    for (i = 0; i < 2; i++)
    {
        send_buf[i+3] = NOW_RMP_1.tem_array[i];
        send_buf[i+5] = NOW_RMP_2.tem_array[i];
    }

	for (int i = 0; i < 4; i++)
	{
		send_buf[i+7] = REAL_ANGLE.tem_array[i];
	}
	

    // 预留控制指令
	send_buf[3 + Length - 1] = ctrlFlag;              // send_buf[11]
      
    // 设置校验值、消息尾
    send_buf[3 + Length] = serial_get_crc8_value(send_buf, 3 + Length);	//send_buf[12]
    send_buf[3 + Length + 1] = serial_ender[0];
    send_buf[3 + Length + 2] = serial_ender[1];

    USART_Send_String(send_buf,sizeof(send_buf));
}


void USART_Send_String(unsigned char *p, short sendSize)
{ 
	static int length =0;
	while(length<sendSize)
	{   
		//如果使用不是USART1则更改成相应的，比如USART3，这里有两处修改
		while( !(USART1->SR&(0x01<<7)) );//发送缓冲区为空
		USART1->DR=*p;                   
		p++;
		length++;
	}
	length =0;
}


unsigned char serial_get_crc8_value(unsigned char *tem_array, unsigned char len)
{
	unsigned char crc = 0;
	unsigned char i;
	while(len--)
	{
		crc ^= *tem_array++;
		for(i = 0; i < 8; i++)
		{
			if(crc&0x01)
				crc=(crc>>1)^0x8C;
			else
				crc >>= 1;
		}
	}
	return crc;
}

