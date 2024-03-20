#include "includes.h"


/*-----------------------------------发送数据------------------------------------------*/
float data[K];
unsigned char sendData[K*2+4];


void creatSendData(float *data , unsigned char *sendData , int length)
{
	union Swap t;
	int i;
	const unsigned char header[2] = {0x55 , 0xaa};
	const unsigned char ender[2] = {0xcc , 0xbb};
	for( i = 0 ; i < length * 2 + 4 ; i++)//将数组初始化为全部均为0x00
	{
		sendData[i] = 0x00;
	}
	
	sendData[0] = header[0];//添加帧头帧尾
	sendData[1] = header[1];
	
	for( i = 0 ; i < length ; i++)
	{
		t.s = data[i];
		sendData[ 2 * i + 2] = t.swap[0];
		sendData[ 2 * i + 3] = t.swap[1];
	}
	
	sendData[length * 2 + 2] = ender[0];
	sendData[length * 2 + 3] = ender[1];
	
}



void SendData(POT_POSTION *pot_data)
{
	int t;
	data[0] = (float)(ROBOT_REAL_POS_DATA.POS_X-11000);
	data[1] = (float)(ROBOT_REAL_POS_DATA.POS_Y);
	data[2] = (float)(ROBOT_REAL_POS_DATA.POS_YAW*100);
	data[3] = (float)pot_data->left_x_position;
	data[4] = (float)pot_data->left_y_position;
	data[5] = (float)pot_data->middle_x_position;
	data[6] = (float)pot_data->middle_y_position;
	data[7] = (float)pot_data->right_x_position;
	data[8] = (float)pot_data->right_y_position;
	data[9] = (float)2;
	creatSendData(data,sendData,K);

		for(t=0;t<2*K+4;t++)
		{

			USART_SendData(USART_TX, sendData[t]);//向串口1发送数据
			while(USART_GetFlagStatus(USART_TX,USART_FLAG_TC) == RESET);//等待发送结束
		}
}


/*-----------------------------------接收数据------------------------------------------*/

//数据接收暂存区
unsigned char  receiveBuff[128];
//通信协议常量
const unsigned char header[2]  = {0x55, 0xaa};
const unsigned char ender[2]   = {0x0d, 0x0a};

POT_POSTION pot_data;


union receiveData
{
	short d;
	unsigned char data[2];
}left_x,left_y,middle_x,middle_y,right_x,right_y;


short Parse_Data(unsigned char *receiveData ,int index)
{
		union Swap t;
    t.swap[0] = receiveData[index];
    t.swap[1] = receiveData[index + 1];
    return t.s;
}

/**************************************************************************
函数功能：计算八位循环冗余校验，被usartSendData和usartReceiveOneData函数调用
入口参数：数组地址、数组大小
返回  值：无
**************************************************************************/
unsigned char getCrc8(unsigned char *ptr, unsigned short len)
{
	unsigned char crc;
	unsigned char i;
	crc = 0;
	while(len--)
	{
		crc ^= *ptr++;
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


/**************************************************************************
函数功能：通过串口中断服务函数，获取上位机发送的左右轮控制速度、预留控制标志位，分别存入参数中
入口参数：左轮轮速控制地址、右轮轮速控制地址、预留控制标志位
返回  值：无特殊意义
**************************************************************************/
int usartReceiveOneData(int *left_x_pos,int *left_y_pos,int *middle_x_pos,int *middle_y_pos,int *right_x_pos,int *right_y_pos,unsigned char *p_crtlFlag)
{
	unsigned char USART_Receiver              = 0;          //接收数据
	static unsigned char checkSum             = 0;
	static unsigned char USARTBufferIndex     = 0;
	static short j=0,k=0;
	static unsigned char USARTReceiverFront   = 0;
	static unsigned char Start_Flag           = START;      //一帧数据传送开始标志位
	static short dataLength                   = 0;

	USART_Receiver = USART_ReceiveData(USART_RX);
	//接收消息头
	if(Start_Flag == START)
	{
		if(USART_Receiver == 0xaa)                             //buf[1]
		{
			if(USARTReceiverFront == 0x55)        //数据头两位 //buf[0]
			{
				Start_Flag = !START;              //收到数据头，开始接收数据
				//printf("header ok\n");
				receiveBuff[0]=header[0];         //buf[0]
				receiveBuff[1]=header[1];         //buf[1]
				USARTBufferIndex = 0;             //缓冲区初始化
				checkSum = 0x00;				  //校验和初始化
			}
		}
		else 
		{
			USARTReceiverFront = USART_Receiver;  
		}
	}
	else
  {
		switch(USARTBufferIndex)
		{
			case 0://接收的长度
				receiveBuff[2] = USART_Receiver;
				dataLength     = receiveBuff[2];            //buf[2]
				USARTBufferIndex++;
				break;
			case 1://接收所有数据，并赋值处理 
				receiveBuff[j + 3] = USART_Receiver;        //buf[3] - buf[14]					
				j++;
				if(j >= dataLength)                         
				{
					j = 0;
					USARTBufferIndex++;
				}
				break;
			case 2://接收校验值信息
				receiveBuff[3 + dataLength] = USART_Receiver;
				checkSum = getCrc8(receiveBuff, 3 + dataLength);
				  // 检查信息校验值
				if (checkSum != receiveBuff[3 + dataLength]) //buf[15]
				{
					printf("Received data check sum error!");
					return 0;
				}
				USARTBufferIndex++;
				break;
				
			case 3://接收信息尾
				if(k==0)
				{
					//数据0d     buf[10]  无需判断
					k++;
				}
				else if (k==1)
				{
					//数据0a     buf[11] 无需判断

					//进行速度赋值操作					
					 for(k = 0; k < 2; k++)
					{
						left_x.data[k]  = receiveBuff[k + 3]; //buf[3]  buf[4]
						left_y.data[k] = receiveBuff[k + 5]; //buf[5]  buf[6]
						middle_x.data[k] = receiveBuff[k + 7]; //buf[7]  buf[8]
						middle_y.data[k] = receiveBuff[k + 9];//buf[9]  buf[10]
						right_x.data[k] = receiveBuff[k + 11];//buf[11]  buf[12]
						right_y.data[k] = receiveBuff[k + 13];//少赋值了一个值
					}				
					
					//速度赋值操作
					*left_x_pos  = (int)left_x.d;
					*left_y_pos = (int)left_y.d;
					*middle_x_pos  = (int)middle_x.d;
					*middle_y_pos = (int)middle_y.d;
					*right_x_pos  = (int)right_x.d;
					*right_y_pos = (int)right_y.d;
					
					//ctrlFlag
					*p_crtlFlag = receiveBuff[9];                //buf[15]
					
					//-----------------------------------------------------------------
					//完成一个数据包的接收，相关变量清零，等待下一字节数据
					USARTBufferIndex   = 0;
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




