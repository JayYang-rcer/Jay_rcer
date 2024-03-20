#include "includes.h"


// 真实位置数据
ROBOT_REAL_POS ROBOT_REAL_POS_DATA = {0, 0, 0};

// 偏航角位置偏置量
float OFFSET_YAW = 0;


// 全场定位初始化
void Location_Init(void)
{ 
	Action_Uart_init(115200);
}


extern int start_flag;
// 在串口3中断中使用
void USART3_IRQHandler(void)
{
	static uint8_t ch;
	static union
	{
	 uint8_t data[24];
	 float ActVal[6];
	}posture;
	
	static uint8_t count = 0;
  static uint8_t i = 0;
	
	start_flag = 1;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)   
	{
		ch = USART_ReceiveData(USART3);

		switch(count)
		{
			case 0:
			{
				if(ch == 0x0d) count ++;
				else count = 0;
			}
			break;
				 
			case 1:
			{
				if(ch == 0x0a)
				{
					i = 0;
					count ++;
				}
				else if(ch == 0x0d);
				else count = 0;
			}
			break;
				 
			case 2:
			{
			 posture.data[i] = ch;
			 i ++;
			 if(i >= 24)
			 {
				 i = 0;
				 count ++;
			 }
			}
			break;
				 
			case 3:
			{
				if(ch == 0x0a) count++;
				else
				count = 0;
			}
			break;
				 
			case 4:
			{
				if(ch == 0x0d)
				{					 
					//更新传感器数据					 
					Update_Action_gl_position(posture.ActVal);
				}
			  count = 0;
			}
			break;
			 
			default:
			{
				count = 0;
			}
			break;		 
		}
		 
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
	}
}


// action全场定位模块定位数据
ACTION_GL_POS ACTION_GL_POS_DATA = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// 更新action全场定位模块的值
void Update_Action_gl_position(float value[6])
{
	// 记录这次的值
	ACTION_GL_POS_DATA.ANGLE_Z = value[0];  // 有用
	ACTION_GL_POS_DATA.ANGLE_X = value[1];
	ACTION_GL_POS_DATA.ANGLE_Y = value[2];
	ACTION_GL_POS_DATA.POS_X   = value[3];  // 有用
	ACTION_GL_POS_DATA.POS_Y   = value[4];  // 有用
	ACTION_GL_POS_DATA.W_Z     = value[5];

	// 偏航角直接赋值（逆时针为正，顺时针为负）
  ROBOT_REAL_POS_DATA.POS_YAW = ACTION_GL_POS_DATA.ANGLE_Z - OFFSET_YAW;
	
	//消除机械误差,赋值X、Y
	ROBOT_REAL_POS_DATA.POS_X = ACTION_GL_POS_DATA.POS_X + INSTALL_ERROR_Y * sin(ROBOT_REAL_POS_DATA.POS_YAW * PI / 180.0f);
	ROBOT_REAL_POS_DATA.POS_Y = ACTION_GL_POS_DATA.POS_Y - INSTALL_ERROR_Y * (cos(ROBOT_REAL_POS_DATA.POS_YAW * PI / 180.0f)-1);
}

