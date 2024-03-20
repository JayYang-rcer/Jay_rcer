#include "includes.h"


TAKE_STATE_ITEMS TAKE_STATE = TAKE_INIT;
CLAW_STATE_ITEMS CLAW_STATE = CLAW_STATE_OPEN;
float cur_point;

void Take_task(void *pvParameters)
{
	float time_counter = 0;
	int step = 1;
	
	while(1)
	{
		switch(TAKE_STATE) 
		{
			/* 初始化 */
			case TAKE_INIT:
				Cylinder_1_OFF();
				Cylinder_2_OFF();
				CLAW_STATE = CLAW_STATE_OPEN;
				break;
			
			
			/* 装填左边 */
			case TAKE_LEFT:
				// 2号气缸收缩
				if (step == 1)
				{
					Cylinder_2_ON();
					step = 2;
				}
				// ak80旋转
				else if (step == 2)
				{
					if(AK80_Planning_Trapezoid_Acc(2.0, 3.0, cur_point, 0.6f*PI, time_counter))
					{
						cur_point = 0.6f*PI;
						time_counter = 0;
						step = 3;
					}
					time_counter += 0.01f;
				}
				// 1号气缸收缩
				else if (step == 3)
				{
					Cylinder_1_OFF();
					step = 4;
					vTaskDelay(1000);
				}
				// 夹爪解锁
				else if(step == 4)
				{
					CLAW_STATE = CLAW_STATE_CLOSE;
					step = 5;
				}
				break;
			
				
			/* 复位 */
			case TAKE_RESET:
				if (step == 5) step = 1;
				// 1号、2号气缸伸开
				if (step == 1)
				{
					Cylinder_1_ON();
					vTaskDelay(1000);
					Cylinder_2_OFF();
					step = 2;
				}
				// ak80旋转
				else if (step == 2)
				{
					if(AK80_Planning_Trapezoid_Acc(2.0, 3.0, cur_point, 0, time_counter))
					{
						cur_point = 0;
						time_counter = 0;
						step = 1;
						TAKE_STATE = TAKE_DONE;
					}
					time_counter += 0.05f;
				}
				break;
			
			
			/* 装填右边 */
			case TAKE_RIGHT:
				
				// 2号气缸收缩
				if (step == 1)
				{
					Cylinder_2_ON();
					step = 2;
				}
				// ak80旋转
				else if (step == 2)
				{
					if(AK80_Planning_Trapezoid_Acc(2.0, 3.0, cur_point, -0.6f*PI, time_counter))
					{
						cur_point = -0.6f*PI;
						time_counter = 0;
						step = 3;
					}
					time_counter += 0.01f;
				}
				// 1号气缸收缩
				else if (step == 3)
				{
					Cylinder_1_OFF();
					step = 4;
					vTaskDelay(1000);
				}
				// 夹爪解锁
				else if(step == 4)
				{
					CLAW_STATE = CLAW_STATE_CLOSE;
					step = 5;
				}
				break;
			
			/* 等待取箭 */
			case TAKE_DONE:
				Cylinder_1_ON();
				Cylinder_2_OFF();
				break;
			
			default:
				break;
		}
		
		
		switch(CLAW_STATE)
		{
			// 张开夹爪
			case CLAW_STATE_OPEN:
				Usart6_Send_Order(CLAW_OPEN);
				break;
			
			// 合上夹爪
			case CLAW_STATE_CLOSE:
				Usart6_Send_Order(CLAW_CLOSE);
				break;
			
			// 防误触发
			case CLAW_STATE_LOCK:
				break;
			
			default:
				break;
		}
		
		vTaskDelay(10);
	}
}



void Usart6_Send_Order(u8 order)
{
	USART_SendData(USART6, 0xFF);delay_ms(10);
	USART_SendData(USART6, 0x09);delay_ms(10);
	USART_SendData(USART6, 0x00);delay_ms(10);
	USART_SendData(USART6, order);delay_ms(10);
	USART_SendData(USART6, 0x00);delay_ms(10);
}

