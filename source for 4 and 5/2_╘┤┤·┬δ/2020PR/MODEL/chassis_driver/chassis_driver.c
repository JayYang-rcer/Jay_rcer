#include "includes.h"


CHASSIS_MOTOR_RPM CHASSIS_MOTOR_RPM_DATA;
ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;

//// 初始化底盘电机配置
//void cd_chassis_driver_init(void)
//{
//	delay_ms(100);
//	CAN_RoboModule_DRV_Reset(0, 0);  // 对0组驱动器进行复位
//	delay_ms(500);                                     
//	CAN_RoboModule_DRV_Config(0, 0, 1, 0);  // 配置为1ms传回一次数据 关闭左右限位功能
//	delay_ms(50); 
//	CAN_RoboModule_DRV_Mode_Choice(0, 0, Velocity_Mode);  // 选择进入速度模式
//	delay_ms(500);
//}

// 4轮机器人坐标系逆运动学
// 世界坐标系
// W：正值-逆时针 负值-顺时针
void Robot_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM)
{
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM = (-COS45 * Vx_RPM + SIN45 * Vy_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM = (-COS45 * Vx_RPM - SIN45 * Vy_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = ( COS45 * Vx_RPM - SIN45 * Vy_RPM + Robot_R*W_RPM) * MS_TO_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = ( COS45 * Vx_RPM + SIN45 * Vy_RPM + Robot_R*W_RPM) * MS_TO_RPM;
}


// 4轮世界坐标系逆运动学
// theta为机器人坐标系x轴与世界坐标系x轴夹角 单位：度
// W：正值-逆时针 负值-顺时针
void World_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM, float theta)
{
	theta = PI * theta / 180.0f;
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM = (-cos(theta-PI/4.0f) * Vx_RPM - sin(theta-PI/4.0f) * Vy_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM = (-cos(theta+PI/4.0f) * Vx_RPM - sin(theta+PI/4.0f) * Vy_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = ( cos(theta-PI/4.0f) * Vx_RPM + sin(theta-PI/4.0f) * Vy_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = ( cos(theta+PI/4.0f) * Vx_RPM + sin(theta+PI/4.0f) * Vy_RPM + Robot_R*W_RPM) * M3508_MS_To_RM;
	
}

#if RM3508
// 控制底盘跟着速度期望值调整速度
void cd_velocity_adjust(void *pdata)
{
	while(1)
	{
//		ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0.2;
//		ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
//		ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;						

	// 速度分解
	World_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
								ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
								ROBOT_TARGET_VELOCITY_DATA.W_RPM,
								ROBOT_REAL_POS_DATA.YAW_POS);
		
//	Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
//								ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
//								ROBOT_TARGET_VELOCITY_DATA.W_RPM);	

	
	// 进行PID计算
	PID_incremental_PID_calculation(&M3508_CHASSIS_MOTOR_PID_RPM[0], M3508_CHASSIS_MOTOR_REAL_INFO[0].RPM, CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM);
	PID_incremental_PID_calculation(&M3508_CHASSIS_MOTOR_PID_RPM[1], M3508_CHASSIS_MOTOR_REAL_INFO[1].RPM, CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM);
	PID_incremental_PID_calculation(&M3508_CHASSIS_MOTOR_PID_RPM[2], M3508_CHASSIS_MOTOR_REAL_INFO[2].RPM, CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM);
	PID_incremental_PID_calculation(&M3508_CHASSIS_MOTOR_PID_RPM[3], M3508_CHASSIS_MOTOR_REAL_INFO[3].RPM, CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM);
	
	// 设置电流
	M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT = M3508_CHASSIS_MOTOR_PID_RPM[0].output;
	M3508_CHASSIS_MOTOR_REAL_INFO[1].TARGET_CURRENT = M3508_CHASSIS_MOTOR_PID_RPM[1].output;
	M3508_CHASSIS_MOTOR_REAL_INFO[2].TARGET_CURRENT = M3508_CHASSIS_MOTOR_PID_RPM[2].output;
	M3508_CHASSIS_MOTOR_REAL_INFO[3].TARGET_CURRENT = M3508_CHASSIS_MOTOR_PID_RPM[3].output;
	

	OSTimeDly(5);
  }
}

#endif

#if RE40
// 控制底盘跟着速度期望值调整速度
void cd_velocity_adjust(void *pdata)
{
	static u8 can_send_order_flag = 1;


	
//	if(MOVING_STATE == MOVING_ROMOTE)  // 若处于遥控控制移动状态 则使用机器人坐标系
//	{
//		Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
//																					ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
//																					ROBOT_TARGET_VELOCITY_DATA.W_RPM);
//	}
//	else  // 若处于自动移动状态 则使用世界坐标系
//	{
//		World_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
//																					ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
//																					ROBOT_TARGET_VELOCITY_DATA.W_RPM,
//																					ROBOT_REAL_POS_DATA.YAW_POS);
//	}

//	World_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
//																				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
//																				ROBOT_TARGET_VELOCITY_DATA.W_RPM,
//																				ROBOT_REAL_POS_DATA.YAW_POS);
//	
   while(1)
   {
    switch(NOW_STATE)
		{
			case MOVING_START_TO_CATCH_1st_BALL_POINT:
			{
				
		    Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					    ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM);
				
			}break;
			
			case MOVING_CATCH_1st_BALL_POINT_TO_CATCH_2nd_BALL_POINT:
			{
				
		    Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					    ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM);

			}break;
			
			case MOVING_CATCH_2nd_BALL_POINT_TO_CATCH_3rd_BALL_POINT:
			{
				
		    Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					    ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM);

			}break;
			
			case MOVING_CATCH_3rd_BALL_POINT_TO_CATCH_4th_BALL_POINT:
			{
				
		    Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					    ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM);

			}break;
			
			case MOVING_CATCH_4th_BALL_POINT_TO_CATCH_5th_BALL_POINT:
			{
				
		    Robot_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					    ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM);

			}break;
			
			default:
			{
			  World_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																				      ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					    ROBOT_TARGET_VELOCITY_DATA.W_RPM,
																					    ROBOT_REAL_POS_DATA.YAW_POS);
				
			}break;
			
		}
		
	// 按期望值调整速度
	// 因为电机接线原因进行转动方向调整
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM = -CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM =  CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = -CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM;
	
	
	// 发送指令
	switch(can_send_order_flag)
	{
		case 1:
		{
			CAN_RoboModule_DRV_Velocity_Mode(0, 1, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 2, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 3, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 4, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM);

			can_send_order_flag = 0;
		}
		break;
		
		case 0:
		{
			CAN_RoboModule_DRV_Velocity_Mode(0, 4, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 3, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 2, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM);
			CAN_RoboModule_DRV_Velocity_Mode(0, 1, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM);

			can_send_order_flag = 1;			
		}
		break;

		default: break;
	}
	//printf("电机运作 SUCCEEDED\r\n");
	  OSTimeDly(5);
  }
}

#endif
