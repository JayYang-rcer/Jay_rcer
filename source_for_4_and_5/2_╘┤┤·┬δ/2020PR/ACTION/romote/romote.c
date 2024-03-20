#include "includes.h"


void romote_action_task(void)
{
	PS2_update_ps2_command();	 // 更新PS2的指令
	
	if(ROMOTE_STATE == ROMOTE_START)
	{
		switch(NOW_STATE)
		{
			case ROMOTE_PREPARATION:
			{
				romote_choose_mode();
			}
			break;
			
			case ROMOTE_RELAY:
			{
				romote_control_relay();
			}
			break;
			
			case ROMOTE_SZ:
			{
				romote_control_moving_for_grab();				
			}
			break;

			case ROMOTE_TZ:
			{
				romote_control_moving_for_throw();					
			}
			break;	
			
			default: break;
		}
	}
	
	else if (ROMOTE_STATE == ROMOTE_FREE)
	{
		// 遥控控制移动
		switch(PS2_KEY)
		{
			case PSB_PAD_UP:  // 上箭头
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -ROMOTE_FREE_Vy;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
			}
			break;
			
			case PSB_PAD_DOWN:  // 下箭头
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = ROMOTE_FREE_Vy;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;				
			}
			break;

			case PSB_PAD_LEFT:  // 左箭头
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = ROMOTE_FREE_Vx*K;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
			}
			break;		

			case PSB_PAD_RIGHT:  // 右箭头
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = (-ROMOTE_FREE_Vx)*K;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
			}
			break;

			case PSB_L1:  // L1
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = ROMOTE_FREE_W;			
			}
			break;

			case PSB_R1:  // R1
			{
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = -ROMOTE_FREE_W;			
			}
			break;
			
			default:  // 未按下规定按键时 
			{
				// 让机器人停止移动
				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
			}
			break;
		}
		
		// 分配路径切换条件
		switch(NOW_STATE)
		{
			case MOVING_START_TO_LINE1:
			{
				moving_select_switch_mode_START_TO_LINE1();  // 选择不同的切换路径方式
			}
			break;

			case MOVING_RZ_TO_SZ:
			{
				moving_select_switch_mode_RZ_TO_SZ(); 
			}
			break;
			
			case MOVING_SZ_TO_WZ:
			{
				moving_select_switch_mode_SZ_TO_WZ();
			}
			break;
			
			case MOVING_WZ_TO_TZ:
			{
				moving_select_switch_mode_WZ_TO_TZ();
			}
			break;
			
			case MOVING_SZ_TO_TZ:
			{
				moving_select_switch_mode_SZ_TO_TZ();
			}
			break;
			
			case MOVING_TZ_TO_SZ:
			{
				moving_select_switch_mode_TZ_TO_SZ();
			}
			break;

			case MOVING_LINE1_TO_SZ:
			{
				moving_select_switch_mode_LINE1_TO_SZ();
			}
			break;
			
			default: break;
		}
		
		// 判断切换并退出遥控模式
		if((*switch_determine)() == 1)
		{
			// 重置PID
			PID_reset_PID(&arc_pid);
			PID_reset_PID(&line_pid);
			PID_reset_PID(&yaw_pid);
			PID_reset_PID(&point_pid);
			PID_reset_PID(&mix_pid);
			
			PATH_POINT_COUNTER = 0;
			MOVING_STATE = MOVING_ARRIVED;  // 切换移动状态
			
			printf("################## PATH TRACK ARRIVED ##################\r\n");
		}
	}
	
	// 一直有效
	if((PS2_RX < 10)&&(PS2_RY < 10)&&(PS2_KEY == PSB_L2))  // L2 右摇杆右上移动
	{
		MR2_STATE = ARRIVED;  // MR2到达 让机器人夹到兽骨后直接前往投掷区
	}
	if(MOVING_STATE == MOVING_GOING)
	{
		if((PS2_RX < 10)&&(PS2_RY < 10)&&(PS2_KEY == PSB_SELECT))  // SELECT 右摇杆右上移动
		{
			MOVING_STATE = MOVING_ROMOTE;  // 切换到手动遥控移动
			ROMOTE_STATE = ROMOTE_FREE;
			PATH_POINT_COUNTER = PATH_POINT_SUM-1;
		}
	}
}


/*WAITING_CHANGE*/
void romote_choose_mode(void)
{
	#if TEST_ALL_PATH
	
	// 配置位置偏置量
	ROBOT_REAL_POS_DATA.X_POS = START_OFFSET_X;
	ROBOT_REAL_POS_DATA.Y_POS = START_OFFSET_Y;
	YAW_OFFSET = START_OFFSET_YAW;
	
	// 设置状态
	OPTAINAL_MODE = MODE_MOVING_START_TO_LINE1;
	MR2_STATE = ARRIVED;  // 让机器人夹到兽骨后直接前往投掷区
	ROMOTE_STATE = ROMOTE_OVER;
	
	return;
	
	#endif

	switch(PS2_KEY)
	{
		case PSB_SQUARE:  // 方形按键
		{
			// 配置位置偏置量
			#if K == 1
			ROBOT_REAL_POS_DATA.X_POS = START_OFFSET_X*K;
			ROBOT_REAL_POS_DATA.Y_POS = START_OFFSET_Y;
			OFFSET_YAW = START_OFFSET_YAW;	
			#else
			ROBOT_REAL_POS_DATA.X_POS = START_OFFSET_X*K;
			ROBOT_REAL_POS_DATA.Y_POS = START_OFFSET_Y;
			OFFSET_YAW = START_OFFSET_YAW*K;		
			#endif
			
			// 设置状态
			OPTAINAL_MODE = MODE_MOVING_START_TO_LINE1;  // 从启动区到交接点移动
			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态	

			printf("################### GO TO MOVING_START_TO_LINE1 ###################\r\n");			
		}
		break;
		
		case PSB_CROSS:  // 叉按键
		{
			// 配置位置偏置量
			ROBOT_REAL_POS_DATA.X_POS = LINE1_OFFSET_X*K;
			ROBOT_REAL_POS_DATA.Y_POS = LINE1_OFFSET_Y;
			OFFSET_YAW = LINE1_OFFSET_YAW*K;	

			// 设置状态
			OPTAINAL_MODE = MODE_ROMOTE_RELAY;  // 从界线1到交接点，并进行交接动作
			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态	

			printf("################### GO TO ROMOTE_RELAY ###################\r\n");				
		}
		break;

		case PSB_TRIANGLE:  // 三角按键
		{
			// 配置位置偏置量
			ROBOT_REAL_POS_DATA.X_POS = LINE1_OFFSET_X*K;
			ROBOT_REAL_POS_DATA.Y_POS = LINE1_OFFSET_Y;
			OFFSET_YAW = LINE1_OFFSET_YAW*K;	

			// 设置状态
			OPTAINAL_MODE = MODE_MOVING_LINE1_TO_SZ;  // 从界线1到兽骨位置
			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态			

			printf("################### GO TO MOVING_LINE1_TO_SZ ###################\r\n");				
		}
		break;		
		
		default: break;
	}
}


void romote_control_relay(void)
{
	switch(PS2_KEY)
	{
		case PSB_CIRCLE:  // 圆形
		{
			// 让机器人停止移动
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
			
			RELAY_ACTION_FLAG = 1;
			
			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态	
		}
		break;
		
		default:  // 未按下规定按键时 
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = (-RELAY_Vx)*K;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -RELAY_Vy;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
		}
		break;
	}
}


void romote_control_moving_for_grab(void)
{
	#if TEST_ALL_PATH
	
	ROMOTE_STATE = ROMOTE_OVER;
	
	return;
	
	#endif
	
	switch(PS2_KEY)
	{
		case PSB_PAD_UP:  // 上箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -ROMOTE_SZ_Vy;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
		}
		break;
		
		case PSB_PAD_DOWN:  // 下箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = ROMOTE_SZ_Vy;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;				
		}
		break;

		case PSB_PAD_LEFT:  // 左箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = ROMOTE_SZ_Vx;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
		}
		break;		

		case PSB_PAD_RIGHT:  // 右箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = -ROMOTE_SZ_Vx;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
		}
		break;

		case PSB_L1:  // L1
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = ROMOTE_SZ_W;			
		}
		break;

		case PSB_R1:  // R1
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = -ROMOTE_SZ_W;			
		}
		break;
		
		case PSB_CIRCLE:  // 圆形
		{
			// 让机器人停止移动
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;

			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态	
		}
		break;
		
		default:  // 未按下规定按键时 
		{
			// 让机器人停止移动
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
		}
		break;
		
		 
	}
}

void romote_control_moving_for_throw(void)
{
	#if TEST_ALL_PATH
	
	ROMOTE_STATE = ROMOTE_OVER;
	
	return;
	
	#endif
	
	switch(PS2_KEY)
	{
		case PSB_PAD_UP:  // 上箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = ROMOTE_TZ_Vy;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
		}
		break;
		
		case PSB_PAD_DOWN:  // 下箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -ROMOTE_TZ_Vy;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;				
		}
		break;

		case PSB_PAD_LEFT:  // 左箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = -ROMOTE_TZ_Vx;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
		}
		break;		

		case PSB_PAD_RIGHT:  // 右箭头
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = ROMOTE_TZ_Vx;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;			
		}
		break;

		case PSB_L1:  // L1
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = ROMOTE_TZ_W;			
		}
		break;

		case PSB_R1:  // R1
		{
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = -ROMOTE_TZ_W;			
		}
		break;

		case PSB_CIRCLE:  // 圆形
		{
			// 让机器人停止移动
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;

			ROMOTE_STATE = ROMOTE_OVER;  // 跳出遥控状态	
		}
		break;
		
		default:  // 未按下规定按键时 
		{
			// 让机器人停止移动
			ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
			ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
		}
		break;
	}

}
