#include "includes.h"


unsigned int CAN_Time_Out = 0;
unsigned char can_tx_success_flag = 0;
float T;
static void CAN_Delay_Us(unsigned int t)
{
	int i;
	for(i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}




void Odrive_Control_pos1(CAN_TypeDef *CANx, u8 motor_num, int pos, float vel_limit, float accel_limit, float decel_limit)
{
	int cmd_id[3] = {0x00B, 0x011, 0x012}; // 在github上的md中的对应id
	CanTxMsg tx_message;
	Pos_limit set_point_limit;
	
	for(int i=2;i>-1;i--)
	{	
		tx_message.StdId = (motor_num << 5) + cmd_id[i]; 
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;
		if(i==0)
			{
				tx_message.Data[0] = pos;	
				tx_message.Data[1] = pos>>8;	
				tx_message.Data[2] = pos>>16;	
				tx_message.Data[3] = pos>>24;	
				tx_message.Data[4] = 0;
				tx_message.Data[5] = 0;
				tx_message.Data[6] = 0;
				tx_message.Data[7] = 0;
			}
			
			if(i==1)
			{
				set_point_limit.ActVal =  vel_limit;
				tx_message.Data[0] = set_point_limit.data[0];	
				tx_message.Data[1] = set_point_limit.data[1];	
				tx_message.Data[2] = set_point_limit.data[2];	
				tx_message.Data[3] = set_point_limit.data[3];	
				tx_message.Data[4] = 0;
				tx_message.Data[5] = 0;
				tx_message.Data[6] = 0;
				tx_message.Data[7] = 0;
			}
			
			if(i==2)
			{
				set_point_limit.ActVal =  accel_limit;
				tx_message.Data[0] = set_point_limit.data[0];	
				tx_message.Data[1] = set_point_limit.data[1];	
				tx_message.Data[2] = set_point_limit.data[2];	
				tx_message.Data[3] = set_point_limit.data[3];	
				
				set_point_limit.ActVal =  decel_limit;
				tx_message.Data[4] = set_point_limit.data[0];	
				tx_message.Data[5] = set_point_limit.data[1];	
				tx_message.Data[6] = set_point_limit.data[2];	
				tx_message.Data[7] = set_point_limit.data[3];	
			}

		can_tx_success_flag = 0;
		CAN_Transmit(CANx,&tx_message);

		CAN_Time_Out = 0;
		while(can_tx_success_flag == 0)
		{
				CAN_Delay_Us(1);
				CAN_Time_Out++;
				if(CAN_Time_Out>100)
				{
						break;
				}
		}
	}
}

void Odrive_Control_Vel(CAN_TypeDef *CANx, u8 motor_num, int vel)
{
		int cmd_id[3] = {0x00D, 0x00E, 0x012}; // 在github上的md中的对应id
		CanTxMsg tx_message;

		tx_message.StdId = (motor_num << 5) + cmd_id[0]; 
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;

		vel *= 100;
		tx_message.Data[0] = vel;	
		tx_message.Data[1] = vel>>8;	
		tx_message.Data[2] = vel>>16;	
		tx_message.Data[3] = vel>>24;	
		tx_message.Data[4] = 0;
		tx_message.Data[5] = 0;
		tx_message.Data[6] = 0;
		tx_message.Data[7] = 0;


		can_tx_success_flag = 0;
		CAN_Transmit(CANx,&tx_message);

		CAN_Time_Out = 0;
		while(can_tx_success_flag == 0)
		{
				CAN_Delay_Us(1);
				CAN_Time_Out++;
				if(CAN_Time_Out>100)
				{
						break;
				}
		}
	
}

void Odrive_Control_Cur(CAN_TypeDef *CANx, u8 motor_num, int cur)
{
		int cmd_id[3] = {0x00D, 0x00E, 0x012}; // 在github上的md中的对应id
		CanTxMsg tx_message;

		tx_message.StdId = (motor_num << 5) + cmd_id[1]; 
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;

		cur /= 10;  //mA
		tx_message.Data[0] = cur;	
		tx_message.Data[1] = cur>>8;	
		tx_message.Data[2] = cur>>16;	
		tx_message.Data[3] = cur>>24;	
		tx_message.Data[4] = 0;
		tx_message.Data[5] = 0;
		tx_message.Data[6] = 0;
		tx_message.Data[7] = 0;


		can_tx_success_flag = 0;
		CAN_Transmit(CANx,&tx_message);

		CAN_Time_Out = 0;
		while(can_tx_success_flag == 0)
		{
				CAN_Delay_Us(1);
				CAN_Time_Out++;
				if(CAN_Time_Out>100)
				{
						break;
				}
		}
	
}

/****************************************************************************************
                                    反馈指令
Get_Encoder_Estimates_Tx(CAN_TypeDef *CANx, u8 drive_num)

驱动器反馈为 call & respond
此函数为call，填入需要查看反馈的电机编号，发送
反馈数据在can中断中进行读取
反馈数据为位置与速度

drive_num   取值范围 1-12

*****************************************************************************************/

	void Get_Encoder_Estimates_Tx(CAN_TypeDef *CANx, u8 drive_num)
{
    CanTxMsg tx_message;
    tx_message.StdId = (drive_num << 5) + 0x009;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0;	
    tx_message.Data[1] = 0;	
    tx_message.Data[2] = 0;	
    tx_message.Data[3] = 0;	
    tx_message.Data[4] = 0;
    tx_message.Data[5] = 0;
    tx_message.Data[6] = 0;
    tx_message.Data[7] = 0;
	
		can_tx_success_flag = 0;
    CAN_Transmit(CANx,&tx_message);
		
		CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
            break;
        }
    }
}
/****************************************************************************************
                                    反馈指令
Get_Encoder_Estimates_Rx(CanRxMsg * msg)

此函数在can接收中断中自动调用，

Encoder_Estimates联合体数据结构

bldc_motor[num].ActVal[0] 为num的位置
bldc_motor[num].ActVal[1] 为num的速度

num: 1-12
*****************************************************************************************/

Encoder_Estimates bldc_motor[13];

void Get_Encoder_Estimates_Rx(CanRxMsg * msg)
{
	u8 i = 0;
	switch(msg->StdId)
	{
		case 0x020*1 + 0x009:					//0x01      0x020 * num +0x009
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[1].data[i] = msg->Data[i];
			} 
//		printf("encoder_pos_1:%f\r\n",bldc_motor[1].ActVal[0]);
		};break;
		
		case 0x020*2 + 0x009:					//0x02
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[2].data[i] = msg->Data[i];
			}
//		printf("encoder_pos_2:%f\r\n",bldc_motor[2].ActVal[0]);			
		};break;

		case 0x020*3 + 0x009:					//0x03
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[3].data[i] = msg->Data[i];
			}
//		printf("encoder_pos_3:%f\r\n",bldc_motor[3].ActVal[0]);
		};break;
		
		case 0x020*4 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[4].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*5 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[5].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*6 + 0x009:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[6].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*7 + 0x009:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[7].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*8 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[8].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*9 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[9].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*10 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[10].data[i] = msg->Data[i];
			}
		};break;

		case 0x020*11 + 0x009:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[11].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*12 + 0x009:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor[12].data[i] = msg->Data[i];
			}
		};break;
		
	}
}
/****************************************************************************************
                                    电流反馈
Get_IQ_Tx(CAN_TypeDef *CANx, u8 drive_num)

Iq Setpoint 0   反馈你发送的电流目标值，不需要用
Iq Measured	4		反馈的实际电流

驱动器反馈为 call & respond
此函数为call，填入需要查看反馈的电机编号，发送
反馈数据在can中断中进行读取
反馈数据为电流

drive_num   取值范围 1-12

*****************************************************************************************/


void Get_IQ_Tx(CAN_TypeDef *CANx, u8 drive_num)
{
    CanTxMsg tx_message;
    tx_message.StdId = (drive_num << 5) + 0x014;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0;	
    tx_message.Data[1] = 0;	
    tx_message.Data[2] = 0;	
    tx_message.Data[3] = 0;	
    tx_message.Data[4] = 0;
    tx_message.Data[5] = 0;
    tx_message.Data[6] = 0;
    tx_message.Data[7] = 0;
	
		can_tx_success_flag = 0;
    CAN_Transmit(CANx,&tx_message);
		
		CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
            break;
        }
    }
}


/****************************************************************************************
                                    反馈指令
Get_IQ_Rx(CanRxMsg * msg)

此函数在can接收中断中自动调用，

Encoder_Estimates联合体数据结构

bldc_motor_iq[num].Setpoint[0] 为num的设置值
bldc_motor_iq[num].Measured[1] 为num的测量值

num: 1-12
*****************************************************************************************/

IQ bldc_motor_iq[13];

void Get_IQ_Rx(CanRxMsg * msg)
{
	u8 i = 0;
	switch(msg->StdId)
	{
		case 0x020*1 + 0x014:					//0x01      0x020 * num +0x014
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[1].data[i] = msg->Data[i];
			}	
//		printf("IQ_1:%f\r\n",bldc_motor_iq[1].ActVal[0]);			
		};break;
		
		case 0x020*2 + 0x014:					//0x02
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[2].data[i] = msg->Data[i];
			} 
//		printf("IQ_1:%f\r\n",bldc_motor_iq[2].ActVal[0]);
		};break;

		case 0x020*3 + 0x014:					//0x03
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[3].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*4 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[4].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*5 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[5].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*6 + 0x014:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[6].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*7 + 0x014:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[7].data[i] = msg->Data[i];
			}
//			printf("IQ_1:%f\r\n",bldc_motor_iq[7].ActVal[0]);	
		};break;
		case 0x020*8 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[8].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*9 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[9].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*10 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[10].data[i] = msg->Data[i];
			}
		};break;

		case 0x020*11 + 0x014:				
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[11].data[i] = msg->Data[i];
			}
		};break;
		
		case 0x020*12 + 0x014:					
		{
			for(i=0;i<8;i++)
			{
				bldc_motor_iq[12].data[i] = msg->Data[i];
			}
		};break;
		
	}
}


void odrive_control(u8 motor_num ,float t,float Target_t ,int motor_VEL)
{
	static int i = 0;
	static float LAST_time;
	float real_t;
	if(i==0)
	{
		LAST_time = t;
		i = i+1;
	}
	real_t = t - LAST_time;
	T = real_t;
	
	if(real_t<Target_t)
	{
		Odrive_Control_Vel(CAN2,motor_num,motor_VEL);
	}
	else if(real_t>Target_t)
	{
		Odrive_Control_Vel(CAN2,motor_num,4000*9*0);
		i = 0;	
		ROBOT.KICK_STATE = KICK_SUCCEED;
	}
}

