#include "includes.h"


#if USE_DATASCOPE
//DataScope任务
void DataScope_task(void *pvParameters)
{
	unsigned char i;          //计数变量
	unsigned char Send_Count; //串口需要发送的数据个数
	
	while(1)
	{
		DataScope_Get_Channel_Data(M3508_SHOOT_MOTOR_REAL_INFO[0].REAL_ANGLE, 1);
		Send_Count = DataScope_Data_Generate(1);
		for( i = 0 ; i < Send_Count; i++) 
		{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
		vTaskDelay(10);
	}
}
#endif



#if USE_DEBUG
//Debug任务
void Debug_task(void *pvParameters)
{		
	
}
#endif
