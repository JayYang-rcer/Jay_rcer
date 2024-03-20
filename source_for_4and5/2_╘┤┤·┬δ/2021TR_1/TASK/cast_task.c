#include "includes.h"


int BottomSwitch_State = 1;
int TopSwitch_State = 1;
float cast_target_v = CAST_V_MIN;
CAST_STATE_ITEMS CAST_STATE = CAST_STATE_HOLD;

// 抬升
void Cast_task(void *pvParameters)
{	
	float TARGET_RPM;
	
	while(1)
	{
		// 读取底部轻触开关状态
		BottomSwitch_State = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
		
		switch(CAST_STATE)
		{
			// HOLD住	
			case CAST_STATE_HOLD:
				CAST_MOTOR_TARGET_RPM = 0;
				break;
			
			// 抬升至顶部
			case CAST_STATE_TOP:
				if(!TopSwitch_State)
				{
					CAST_STATE = CAST_STATE_HOLD;
					Throwing_Institution_Control(SET_TI, 0, 0, 0, 0);
					CTRL_DES.rpm_des = RPM_MIN+300; 
					CTRL_DES.pitch_des = -22.5f;
					ROBOT_STATE = ROBOT_STATE_THROW;
					cast_target_v = CAST_V_MIN;
				}
				else
				{
					cast_target_v = fminf(fmaxf(CAST_V_MIN, cast_target_v), CAST_V_MAX); // 限幅
					CAST_MOTOR_TARGET_RPM = cast_target_v;
				}
				break;
			
			// 下降至底部
			case CAST_STATE_BOTTOM:
				if(!BottomSwitch_State)
				{
					CAST_STATE = CAST_STATE_HOLD;
					ROBOT_STATE = ROBOT_STATE_RELOAD;
					cast_target_v = CAST_V_MIN;
				}
				else
				{
					cast_target_v = fminf(fmaxf(CAST_V_MIN, cast_target_v), CAST_V_MAX);
					CAST_MOTOR_TARGET_RPM = -cast_target_v;
				}
				break;
				
			default:
				break;
		}
		
		TARGET_RPM = CAST_MOTOR_TARGET_RPM*MS_TO_RPM;							// 单位转换
		TARGET_RPM = fminf(fmaxf(-6000.0f, TARGET_RPM), 6000.0f); // 限幅
		
		PID_incremental_PID_calculation(&M3508_CAST_MOTOR_PID_RPM, M3508_CAST_MOTOR_REAL_INFO.RPM, TARGET_RPM);
		M3508_CAST_MOTOR_REAL_INFO.TARGET_CURRENT = M3508_CAST_MOTOR_PID_RPM.output;
		
		shoot_m3508_send_motor_currents();
		
		vTaskDelay(10);
	}
}

