#include "headfiles.h"


TaskHandle_t Main_Handler;
TaskHandle_t PStwo_Handler;
TaskHandle_t U3Motor_Driver_Handler;

const CTRL_DES_DEF CTRL_RESET = {0, 0, RPM_MIN, 0};
CTRL_DES_DEF CTRL_DES = {0, 0, RPM_MIN, 0};

int TopSwitch_State = 1;

extern TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters)
{
	bsp_init();											//初始化	
	
	taskENTER_CRITICAL();           //进入临界区
	
	xTaskCreate((TaskFunction_t )Main_task,     
							(const char*    )"Main_task",   
							(uint16_t       )MAIN_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )MAIN_TASK_PRIO,
							(TaskHandle_t*  )&Main_Handler);
#if CUR_ID
	xTaskCreate((TaskFunction_t )PStwo_task,     
							(const char*    )"PStwo_task",   
							(uint16_t       )PSTWO_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )PSTWO_TASK_PRIO,
							(TaskHandle_t*  )&PStwo_Handler);
#endif
#if USE_MIT_DRIVER
	xTaskCreate((TaskFunction_t )U3Motor_Driver_task,     
							(const char*    )"U3Motor_Driver_task",   
							(uint16_t       )PSTWO_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )PSTWO_TASK_PRIO,
							(TaskHandle_t*  )&U3Motor_Driver_Handler);
#endif
	vTaskDelete(StartTask_Handler);

	taskEXIT_CRITICAL();            //退出临界区
	              							
}


float pitch_f, yaw_f;
void Main_task(void *pvParameters)
{
#if (CUR_ID == 0)
	// 发送上位机
	CanTxMsg tx_message;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x01;
	tx_message.StdId = 0x04;
#endif
	
	while(1)
	{
		// 限幅
		CTRL_DES.rpm_des = fminf(fmaxf(RPM_MIN, CTRL_DES.rpm_des), RPM_MAX);
		CTRL_DES.yaw_des = fminf(fmaxf(YAW_MIN, CTRL_DES.yaw_des), YAW_MAX);
		CTRL_DES.pitch_des = fminf(fmaxf(PITCH_MIN, CTRL_DES.pitch_des), PITCH_MAX);
		
		// 偏航俯仰控制
		pitch_f = CTRL_DES.pitch_des/180.0f*PI*9; // 减速比9：1
		yaw_f = CTRL_DES.yaw_des/180.0f*PI*9;
		AK80_motion_control(AK80_PITCH_ID, pitch_f, 0, 45, 5, 0);	
		AK80_motion_control(AK80_YAW_ID, yaw_f, 0, 45, 5, 0);
		
#if (USE_MIT_DRIVER == 0)
		// 摩擦轮转速控制
		TIM_SetCompare1(TIM4, CTRL_DES.rpm_des);	
		TIM_SetCompare2(TIM4, CTRL_DES.rpm_des);
#endif
		
#if (CUR_ID == 0)
		// 限位开关读取
		TopSwitch_State = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
		tx_message.Data[0] = TopSwitch_State;
		CAN_Transmit(CAN1, &tx_message);
#endif
		
		// 射箭控制
		if (CTRL_DES.is_set)
		{
			CTRL_DES.is_set = 0;
			// 舵机抬起
#if (CUR_ID == 0)
			TIM_SetCompare3(TIM3, 800);
#else
			TIM_SetCompare3(TIM3, 2100);
#endif
			vTaskDelay(200);
			// 气缸推进
			Throw_Disable();vTaskDelay(1300); 
#if (CUR_ID == 0)
			AK80_motion_control(AK80_PITCH_ID, pitch_f+4.7f, 0, 45, 5, 0);
#else
			AK80_motion_control(AK80_PITCH_ID, pitch_f-4.7f, 0, 45, 5, 0);
#endif
			vTaskDelay(700);
			// 复位
#if (USE_MIT_DRIVER == 0)
			TIM_SetCompare1(TIM4, CTRL_DES.rpm_des+100);	 
			TIM_SetCompare2(TIM4, CTRL_DES.rpm_des+100);
#endif
#if (CUR_ID == 0)
			TIM_SetCompare3(TIM3, 2000);
#else
			TIM_SetCompare3(TIM3, 1050);
#endif
			Throw_Enable();
		}
		
		vTaskDelay(10);
	}
}



float rpm_des_f;
void U3Motor_Driver_task(void *pvParameters)
{	
	while(1)
	{
		// 速度处理
		rpm_des_f = CTRL_DES.rpm_des - RPM_MIN;
		
		// 速度赋值
#if CUR_ID
		MIT_DRIVER_REAL_INFO[0].TARGET_SPEED = -rpm_des_f;
		MIT_DRIVER_REAL_INFO[1].TARGET_SPEED = rpm_des_f;
#else
		MIT_DRIVER_REAL_INFO[0].TARGET_SPEED = rpm_des_f;
		MIT_DRIVER_REAL_INFO[1].TARGET_SPEED = -rpm_des_f;
#endif
		
		// PID运算
		PID_incremental_PID_calculation(&MIT_DRIVER_PID_INFO[0], MIT_DRIVER_REAL_INFO[0].V_angle, MIT_DRIVER_REAL_INFO[0].TARGET_SPEED);
		PID_incremental_PID_calculation(&MIT_DRIVER_PID_INFO[1], MIT_DRIVER_REAL_INFO[1].V_angle, MIT_DRIVER_REAL_INFO[1].TARGET_SPEED);
		
		// 电流发送
		MIT_Driver_Current_Send(MIT_DRIVER_ID_1, MIT_DRIVER_PID_INFO[0].output);
		MIT_Driver_Current_Send(MIT_DRIVER_ID_2, MIT_DRIVER_PID_INFO[1].output);
		
		vTaskDelay(5);
	}
}




void PStwo_task(void *pvParameters)
{	
	int flag = 0;
	int press_time_counter = 0;	// 长按
	
	while(1)
	{
		// 更新数据
		PS2_update_ps2_command();
		
		// 按键数据处理
		if(PS2_KEY)
		{
			switch(PS2_KEY)
			{
				// 发射
				case PSB_R2:
					if (!flag)
					{
						CTRL_DES.is_set = 1;
						flag = 1;
					}
					break;
				
				// 摩擦轮转速-
				case PSB_L1:
					if (!flag)
					{
						CTRL_DES.rpm_des -= 20;
						flag = 1;
					}
					break;
				
				// 摩擦轮转速+ 
				case PSB_R1:
					if (!flag)
					{
						CTRL_DES.rpm_des += 20;
						flag = 1;
					}
					break;
				
				// 俯仰+
				case PSB_PAD_UP:
					if (!flag)
					{
						CTRL_DES.pitch_des -= 1.5f;
						flag = 1;
					}
					break;
				
				// 俯仰-
				case PSB_PAD_DOWN:
					if (!flag)
					{
						CTRL_DES.pitch_des += 1.5f;
						flag = 1;
					}
					break;
				
				// 偏航-
				case PSB_PAD_LEFT:
					if (!flag)
					{
						CTRL_DES.yaw_des -= 1.5f;
						flag = 1;
					}
					if (press_time_counter > 10)
					{
						CTRL_DES.yaw_des -= 0.3f;
					}
					else press_time_counter++;
					break;
				
				// 偏航+
				case PSB_PAD_RIGHT:
					if (!flag)
					{
						CTRL_DES.yaw_des += 1.5f;
						flag = 1;
					}
					if (press_time_counter > 10)
					{
						CTRL_DES.yaw_des += 0.3f;
					}
					else press_time_counter++;
					break;
				
				// 复位
				case PSB_PINK:
					CTRL_DES = CTRL_RESET;
					Throw_Disable();
					break;
								
				default:
					break;
			}
		}
		else 
		{
			press_time_counter = 0;
			flag = 0;
		}
		
		vTaskDelay(20);
	}
}




//反馈电机信息	
void Upper_update_info(CanRxMsg *msg)
{ 	
	  int yaw_int;
	  int pitch_int;
		switch(msg->StdId)  // 检测标准ID
		{
			case CUR_ID:
			{ 
				yaw_int = (msg->Data[0]<<8)|msg->Data[1];
				pitch_int = (msg->Data[2]<<8)|(msg->Data[3]);
				CTRL_DES.rpm_des = (msg->Data[4]<<8)|(msg->Data[5]);
				CTRL_DES.is_set = msg->Data[6];
				// int -> flaot
				CTRL_DES.yaw_des = uint_to_float(yaw_int, YAW_MIN, YAW_MAX, 16);
				CTRL_DES.pitch_des = uint_to_float(pitch_int, PITCH_MIN, PITCH_MAX, 16);

			}; break;

#if (IS_LEFT_OR_RIGHT == 0)
			case RESET_ID:	// 复位
			{
				CTRL_DES = CTRL_RESET;
				Throw_Disable();
			};break;
#endif
			
			case SET_ID:
				Throw_Enable();
#if (IS_LEFT_OR_RIGHT == 1)
				CTRL_DES.pitch_des = 0;
				CTRL_DES.yaw_des = 0;
				CTRL_DES.rpm_des = RPM_MIN+240;
#endif
				break;
			
			default:break;
		}

}



