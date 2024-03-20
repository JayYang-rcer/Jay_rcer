#include "includes.h"
#include <app_cfg.h>

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	bsp_init();
	//OSStatInit();  //开启统计任务
	
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
	
	//创建轮子运动任务
	OSTaskCreate(cd_velocity_adjust,(void*)0,(OS_STK*)&Wheels_TASK_STK[Wheels_STK_SIZE-1],Wheels_TASK_PRIO); 
	//创建传球任务
	OSTaskCreate(pass_ball_task,(void*)0,(OS_STK*)&PASS_TASK_STK[PASS_STK_SIZE-1],PASS_TASK_PRIO);
	//创建轮运动规划任务
	OSTaskCreate(moving_task,(void*)0,(OS_STK*)&MOVE_TASK_STK[MOVE_STK_SIZE-1],MOVE_TASK_PRIO); 
	//创建状态机任务
	OSTaskCreate(FSM_RUN,(void*)0,(OS_STK*)&FSM_TASK_STK[FSM_STK_SIZE-1],FSM_TASK_PRIO); 
	//创建DEBUG任务
	OSTaskCreate(Debug_by_UART,(void*)0,(OS_STK*)&DEBUG_TASK_STK[DEBUG_STK_SIZE-1],DEBUG_TASK_PRIO); 
	//创建传感器更新任务
	OSTaskCreate(Sensor_Update,(void*)0,(OS_STK*)&SENSOR_TASK_STK[SENSOR_STK_SIZE-1],SENSOR_TASK_PRIO);
  //创建电机任务
  OSTaskCreate(Motor_motion_control,(void*)0,(OS_STK*)&MOTOR_TASK_STK[MOTOR_STK_SIZE-1],MOTOR_TASK_PRIO);	

	OSTaskSuspend(START_TASK_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL();  //退出临界区(开中断)
}

float X[13] = { 0,0,0, 236,  400,  380,    0, -884,-1493, -1728,-1800,-1800,-1800};
float Y[13] = { 0,0,0, 770, 1586, 2430, 3230, 3300, 2714,  1898, 1400, 1400, 1400};

float X1[13] = {-1800,-1800,-1800,-1728,-1413, -844,   0,  380, 400,236,0,0,0};
float Y1[13] = { 1400, 1400, 1400, 1898, 2714, 3300,3230, 2430,1586,770,0,0,0};

float X2[13] = { 0,0,0,   71, 188,  312,  361,  236,-287.1,  -786, -1100,-1100,-1100};
float Y2[13] = { 0,0,0,339.1, 886, 1547, 2226, 2823,  3253,  3504,  3600, 3600, 3600};

float X3[13] = {-1100,-1100,-1100,-501.6,  177,  383,325.6,  210,  99, 29,0,0,0};
float Y3[13] = { 3600, 3600, 3600,  3491, 3061, 2388, 1687, 1000, 467,139,0,0,0};

float X4[13] = {-1800,-1800,-1800,-1658,-1410,-963.5,105.7,  564, 794, 991,1090,1090,1090};
float Y4[13] = { 1400, 1400, 1400, 1979, 2651,  3264, 3101, 2443,1649, 962, 500, 500, 500};

void pass_ball_task(void *pdata)
{
	while(1)
	{
		//射球
		shoot_ball_task();
		//机械臂复位
		ARM_RESET();
		
		OSTimeDly(10);
	}
}


void shoot_ball_task(void)
{
	static u8 flow_counter = 0;
  if(ROBOT.SHOOTSTATE == THROW_START)
	{
		switch(flow_counter)
		{
			case 0:
			{
				if(ROBOT.NOW_STATE == THROW_1st_BALL)
				{
	 					ARM_NOW_MOTION = &ARM_SHOOT_MOTION2;
				}
				else
				{
					  ARM_NOW_MOTION = &ARM_SHOOT_MOTION;
				}

	      if(M3508_ARM_MOTOR_REAL_INFO.REAL_ANGLE >= -40.8)return;

				flow_counter = 1;
			}break;	
			case 1:
			{
				//
				switch(ROBOT.NOW_STATE)
				{
					case THROW_1st_BALL:
					TURNPLATE_adjusting(TURNPLATE_1st_SHOOT_POS); //1
					
//					if(PS2_KEY == 15)
//				  {
//							flow_counter = 2;
//				  }
//					if(ABS(robot.angle - (TURNPLATE_1st_SHOOT_POS))>=0.5) return;
					flow_counter = 2;
					break;
					case THROW_2nd_BALL:
					TURNPLATE_adjusting(TURNPLATE_2nd_SHOOT_POS);	//3


					if(PS2_KEY == 15)
				  {
							flow_counter = 2;
				  }
					if(ABS(robot.angle - (TURNPLATE_2nd_SHOOT_POS))>=0.5) return;
					break;
					case THROW_3rd_BALL:
					TURNPLATE_adjusting(TURNPLATE_3rd_SHOOT_POS); //4
//					if(ABS(robot.angle - (TURNPLATE_3rd_SHOOT_POS))>=1) return;

//					if(PS2_KEY != 15) return;
					if(PS2_KEY == 15)
				  {
							flow_counter = 2;
				  }
					break;
					case THROW_4th_BALL:
					TURNPLATE_adjusting(TURNPLATE_4th_SHOOT_POS); //2


//					if(PS2_KEY != 15) return;
					if(PS2_KEY == 15)
				  {
							flow_counter = 2;
				  }
					break;
          if(ABS(robot.angle - (TURNPLATE_4th_SHOOT_POS))>=0.5) return;
					case THROW_5th_BALL:
					TURNPLATE_adjusting(TURNPLATE_5th_SHOOT_POS); //4
//					if(PS2_KEY != 15) return;
					if(PS2_KEY == 15)
				  {
							flow_counter = 2;
				  }
					if(ABS(robot.angle - (TURNPLATE_5th_SHOOT_POS))>=0.5) return;					
					break;
				
			    default: flow_counter = 2;
						break;
				}
				

				
			}break;

			case 2:
			{
				
				if(M3508_ARM_MOTOR_REAL_INFO.REAL_ANGLE >= -80.8)return;
        ARM_NOW_MOTION = &ARM_SHOOT_2_MOTION;
        if(!Get_limit(ARM_ARRIVE_LIMIT)) return;
	      ARM_NOW_MOTION = &ARM_HOLD_MOTION;
				flow_counter ++;
			}
       break;			

			default:
			{

        ROBOT.SHOOTSTATE = THROW_SUCCEEDED;	
				flow_counter = 0;
			}break;			
		}	
	}
}

void ARM_RESET(void)                                         //手臂复位
{
	static u8 flow_counter = 0;

	
	if(ROBOT.SHOOTSTATE == THROW_RESET)
	{
	  switch(flow_counter)
		{
			case 0:
			{	
				
				ARM_NOW_MOTION = &ARM_RESET_MOTION;

				if(!Get_limit(ARM_RESET_LIMIT)) return;
				
				M3508_ARM_MOTOR_REAL_INFO.REAL_ANGLE = ARM_INIT_POS;  /*WAITING_DEBUG*/

				ARM_NOW_MOTION = &ARM_HOLD_MOTION;
				
				flow_counter ++;
			}break;
				
			default:
			{
				ROBOT.SHOOTSTATE = THROW_RESET_SUCCEEDED;	
				flow_counter = 0;
			}
		}
	}
}

void romote_action_task(void)
{
	PS2_update_ps2_command();	 // 更新PS2的指令
//	moving_yaw_adjusting(0);

	if(ROBOT.SHOOTSTATE == THROW_START)
	{
		switch(PS2_KEY)
		{
			case PSB_PAD_UP:  // 上箭头
			{
					switch(ROBOT.NOW_STATE)
				{

					case THROW_2nd_BALL:
					TIM12->CCR1 = 1175;
			    TIM12->CCR2 = 1175;
					break;
					
					case THROW_3rd_BALL:
					TIM12->CCR1 = 1192;
			    TIM12->CCR2 = 1192;
					break;
					
					case THROW_4th_BALL:
					TIM12->CCR1 = 1165;
			    TIM12->CCR2 = 1165;
					
					case THROW_5th_BALL:
					TIM12->CCR1 = 1175;
			    TIM12->CCR2 = 1175;

					break;
				
			    default: break;
				}
			}
			break;
			
			case PSB_PAD_DOWN:  // 下箭头
			{
				 	TIM12->CCR1 = 0;
			    TIM12->CCR2 = 0;
			}
			break;
			
			default:  // 未按下规定按键时 
			{
					


			}
			break;
		}

		
	}
	if((ROBOT.NOW_STATE == KICK_1st || ROBOT.NOW_STATE == KICK_2nd)&&ROBOT.PUT_LIYU==ok_put)
	{
		INIT_X = ROBOT_REAL_POS_DATA.X_POS;
		INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
		// 遥控控制移动
		switch(PS2_KEY)
		{
			case PSB_PAD_UP:  // 上箭头
			{
					cylinder_on(CATCH_SEAT_2);
				  ROBOT.KICK_SAFE=ok_kick;
			}
			break;
			
			case PSB_PAD_DOWN:  // 下箭头
			{
				 cylinder_off(CATCH_SEAT_3);
         cylinder_off(CATCH_SEAT_1);
				 cylinder_off(CATCH_SEAT_2);
				 ROBOT.KICK_STATE = KICK_END3;
			 	 ROBOT.KICK_SAFE=no_kick;
			}
			break;

			case PSB_PAD_LEFT:  // 左箭头
			{
					cylinder_on(CATCH_SEAT_3); 
				  ROBOT.KICK_SAFE=ok_kick;
			}
			break;		

			case PSB_PAD_RIGHT:  // 右箭头
			{
					cylinder_on(CATCH_SEAT_1);				
				  ROBOT.KICK_SAFE=ok_kick;
			}
			break;

			case PSB_L1:  // L1
			{
          // 让机器人停止移动
				  ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
				  ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				  ROBOT_TARGET_VELOCITY_DATA.W_RPM = -0.05;
				  ROBOT.KICK_STATE = KICK_STOP;
			}
			break;

			case PSB_R1:  // R1
			{			
          // 让机器人停止移动
					ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
					ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
					ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0.05;
          ROBOT.KICK_STATE = KICK_STOP;				
			}
			break;
			
			
			case PSB_L2:  // L2
			{
					if(ROBOT.NOW_STATE == KICK_2nd)
					{
							ROBOT.KICK_STATE = KICK_END1;
							cylinder_off(CATCH_SEAT_3);
				      ROBOT.KICK_SAFE=no_kick;
              cylinder_off(CATCH_SEAT_1);
				      cylinder_off(CATCH_SEAT_2);
					}
			}
			break;

			case PSB_R2:  // R2
			{
				  if(ROBOT.NOW_STATE == KICK_1st && ROBOT.KICK_STATE != STOP_2P && ROBOT.KICK_STATE != STOP_3P && ROBOT.KICK_STATE != KICK_START1 && ROBOT.KICK_STATE != KICK_START2 && ROBOT.KICK_STATE != KICK_START3)
					{
						ROBOT.KICK_STATE = GO_2P;
					}
					if(ROBOT.NOW_STATE == KICK_2nd && ROBOT.KICK_STATE != STOP_2P && ROBOT.KICK_STATE != STOP_3P && ROBOT.KICK_STATE != KICK_START1 && ROBOT.KICK_STATE != KICK_START2 && ROBOT.KICK_STATE != KICK_START3)
					{
						ROBOT.KICK_STATE = GO_3P;
					}
	
			}
			break;
			
		  case PSB_CIRCLE:  // 圆形
		  { 
				  if (ROBOT.KICK_SAFE==ok_kick)
				  {
						 ROBOT.KICK_STATE = KICK_START2;	
				  }
		  }
		  break;
			
			case PSB_CROSS:  // 叉按键
		  { 
					if(ROBOT.NOW_STATE == KICK_1st && ROBOT.KICK_STATE != STOP_START_2P && ROBOT.KICK_STATE != STOP_START_3P )
					{
						 ROBOT.KICK_STATE = KICK_END2;
					}
					if(ROBOT.NOW_STATE == KICK_2nd && ROBOT.KICK_STATE != STOP_START_2P && ROBOT.KICK_STATE != STOP_START_3P )
					{
						 ROBOT.KICK_STATE = BACK_3P;
					}
			}
		  break;
			

			
			case PSB_TRIANGLE:  // 三角按键
		  {
          if (ROBOT.KICK_SAFE==ok_kick)
				  {
				     ROBOT.KICK_STATE = KICK_START3;
				  }
			}
			break;
			
			case PSB_SQUARE:  // 方形按键
		  {
          if (ROBOT.KICK_SAFE==ok_kick)
				  {
				     ROBOT.KICK_STATE = KICK_START1;
				  }
			}
			break;
			
			
			default:  // 未按下规定按键时 
			{
					
//				// 让机器人停止移动
//				ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
//				ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
				  ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;

			}
			break;
		}
	}
}

void KICK_control()
{

	  switch(ROBOT.KICK_STATE)
		{
			case KICK_START1:  
			{			  
					if(ROBOT.NOW_STATE == KICK_2nd)
					{
						 AK80_motion_control(0x03,-0.5f*PI, 0, 18.5, 1, 0);
						 cylinder_on(CATCH_SEAT_3);
						 moving_point_track(-1800,1400);
					}
			}
			break;
			
			case KICK_START2:  
			{			  
					if(ROBOT.NOW_STATE == KICK_2nd)
					{
						 AK80_motion_control(0x02,0.7f*PI, 0, 500, 2, 0);
						 cylinder_on(CATCH_SEAT_1);
						 moving_point_track(-1800,1400);
					}
			}
			break;
			
			case KICK_START3:  
			{

				  if(ROBOT.NOW_STATE == KICK_1st)
					{		
			       AK80_motion_control(0x01,-0.5f*PI, 0, 16.8, 1, 0);
				     cylinder_on(CATCH_SEAT_2);						
						 OSTimeDly(500);
						 ROBOT.KICK_STATE = BACK_2P;
				 	}
					if(ROBOT.NOW_STATE == KICK_2nd)
					{
						 AK80_motion_control(0x01,-0.5f*PI, 0, 17.5, 1, 0);
				     cylinder_on(CATCH_SEAT_2);
						 moving_point_track(-1800,1400);
					}
					
			}
			break;
			
			case KICK_END1:  //复位腿
			{
//			    Odrive_Control_pos1(CAN2,2, 4000*0, 4000*9*1, 4000*9*90, 4000*9*90);
//				  Odrive_Control_pos1(CAN2,2, -4000*0, 4000*9*1, 4000*9*90, 4000*9*90);
					cylinder_off(CATCH_SEAT_3);
          cylinder_off(CATCH_SEAT_1);
				  cylinder_off(CATCH_SEAT_2);
				  AK80_motion_control(0x02,0*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x01,0*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0*PI, 0, 6, 1, 0);

				  ROBOT.KICK_STATE = GO_5_ball;

			}
			break;
			
			case KICK_END2: //复位腿
			{
					cylinder_off(CATCH_SEAT_3);
          cylinder_off(CATCH_SEAT_1);
				  cylinder_off(CATCH_SEAT_2);
					AK80_motion_control(0x01,-0.0f*PI, 0, 10, 1, 0);
					AK80_motion_control(0x02,-0.0f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,-0.0f*PI, 0, 10, 1, 0);
				
				  OSTimeDly(200);
				
				  ROBOT.KICK_STATE = BACK_2P;
         
			}
			break;
			
			case KICK_END3:  //复位腿
			{
				  AK80_motion_control(0x01,0.6f*PI, 0, 6, 1, 0);
//				  AK80_motion_control(0x02,-0.9f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0.6f*PI, 0, 6, 1, 0);
		
			}
			break;
					

			case GO_2P:  //前进二分区
			{
				  AK80_motion_control(0x01,0.6f*PI, 0, 10, 1, 0);
					AK80_motion_control(0x02,-0.0f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,-0.0f*PI, 0, 10, 1, 0);
					cylinder_off(CATCH_SEAT_3);
          cylinder_off(CATCH_SEAT_1);
				  cylinder_off(CATCH_SEAT_2);
          Pathplan(Get_OSTime()/1000,3,10,X2,Y2);
				  if(motion_t>1.0f)
					{
						 moving_yaw_adjusting(ANGLE_2P);
					}
					ROBOT.PUT_LIYU = no_put;

			}
			break;

		
			case BACK_2P:  //后退到启动区
			{
					cylinder_off(CATCH_SEAT_3);
          cylinder_off(CATCH_SEAT_1);
				  cylinder_off(CATCH_SEAT_2);
					AK80_motion_control(0x01,-0.0f*PI, 0, 10, 1, 0);
					AK80_motion_control(0x02,-0.0f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,-0.0f*PI, 0, 10, 1, 0);
				
		      Pathplan(Get_OSTime()/1000,4,10,X3,Y3);

					if(motion_t>2.2f)
					{
						 moving_yaw_adjusting(-90);
					}
				  ROBOT.PUT_LIYU = no_put;

			}
			break;
			
			case GO_3P:  //前进三分区
			{
				  AK80_motion_control(0x01,0.6f*PI, 0, 6, 1, 0);
//				  AK80_motion_control(0x02,-0.9f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0.6f*PI, 0, 6, 1, 0);
          Pathplan(Get_OSTime()/1000,5,10,X,Y);
				  if(motion_t>1)
					{
						 moving_yaw_adjusting(ANGLE_3P);
					}
					ROBOT.PUT_LIYU = no_put;

			}
			break;

		
			case BACK_3P:  //回到原点
			{
					AK80_motion_control(0x01,0.6f*PI, 0, 6, 1, 0);
//				  AK80_motion_control(0x02,-0.9f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0.6f*PI, 0, 6, 1, 0);
					cylinder_off(CATCH_SEAT_3);
          cylinder_off(CATCH_SEAT_1);
				  cylinder_off(CATCH_SEAT_2);
		      Pathplan(Get_OSTime()/1000,5,10,X1,Y1);
					if(motion_t>1)
					{
						 moving_yaw_adjusting(-90);
					}
				  ROBOT.PUT_LIYU = no_put;

			}
			break;
			
			case STOP_3P:  //静止在三分区
			{
				  moving_point_track(-1800,1400);
				  moving_yaw_adjusting(ANGLE_3P);
				  ROBOT.PUT_LIYU = ok_put;
				  ROBOT.KICK_SAFE=ok_kick;
			}
			break;
			
			case STOP_START_3P:  //静止在启动区
			{
					moving_point_track(10,5);
				  moving_yaw_adjusting(-90);
				  ROBOT.PUT_LIYU = ok_put;

			}
			break;
			
			case STOP_2P:  //静止在二分区
			{
					AK80_motion_control(0x01,0.6f*PI, 0, 6, 1, 0);
//				  AK80_motion_control(0x02,-0.9f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0.6f*PI, 0, 6, 1, 0);
//				  moving_point_track(-1100,3600);
				  moving_yaw_adjusting(ANGLE_2P);
				  ROBOT.PUT_LIYU = ok_put;
				  ROBOT.KICK_SAFE=ok_kick;
				
				  if(DELAY_ms(200))
					{
							ROBOT.KICK_STATE = KICK_START3;
					}
				  

			}
			break;
			
			case GO_5_ball:  //去射第五个球
			{
          Pathplan(Get_OSTime()/1000,5,10,X4,Y4);
					if(motion_t>0.6f)
					{ 
						  TURNPLATE_adjusting(-46.5);
							moving_yaw_adjusting(0);
					}
				  ROBOT.PUT_LIYU = no_put;

			}
			break;
			
			case STOP_5_ball:  //静止在二分区
			{
					TIM12->CCR1 = 1172;
			    TIM12->CCR2 = 1172;
          moving_point_track(1128,492);
				  moving_yaw_adjusting(0);
					TURNPLATE_adjusting(-47.5);
				  if(ABS(robot.angle - (-47.5))>=1) return;
				  TURN_MOTOR_TARGET_RPM = 0;
				  ROBOT.NOW_STATE = TO_BALL_5th;
				  ROBOT.KICK_STATE = KICK_STOP;
				  ROBOT.MOVE_STATE = ARRIVE;
				  ROBOT.PUT_LIYU = ok_put;

			}
			break;
			
			case KICK_STOP:
			{
				
			}
			break;
			
			
			case STOP_START_2P:  //静止在启动区
			{
					AK80_motion_control(0x01,0.6f*PI, 0, 6, 1, 0);
//				  AK80_motion_control(0x02,-0.6f*PI, 0, 6, 1, 0);
				  AK80_motion_control(0x03,0.6f*PI, 0, 6, 1, 0);
				  moving_yaw_adjusting(-90);
				  ROBOT.PUT_LIYU = ok_put;

			}
			break;

			
			default:  // 未按下规定按键时 
			{

//				// 让机器人停止移动

//				ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;

			}
			break;
		}
	
}

void TURNPLATE_adjusting(float Target_angle)
{
   static float error;

	 error = Target_angle - robot.angle;
	 PID_position_PID_calculation_by_error(&M2006_TURN_MOTOR_PID_POS,error);
	 
	 TURN_MOTOR_TARGET_RPM = M2006_TURN_MOTOR_PID_POS.output;
	
	
	 
}	

void TURNPLATE_adjust(float P_start, float P_end, float V_start, float V_max ,float V_end,float Rac ,float Rde ,float pos)
{
	float Ssu;   //总路程
	float Sac;   //加速路程
	float Sde;   //减速路程
	float Sco;   //匀速路程
	float Aac;   //加速加速度
	float Ade;   //减速加速度
	float S;     //当前路程
		
	// 计算一些变量
	
	Ssu = ABS(P_end - P_start);    
	Sac = Ssu * Rac;
	Sde = Ssu * Rde;
	Sco = Ssu - Sac - Sde;
	Aac = (V_max * V_max - V_start * V_start) / (2.0f * Sac);
	Ade = (V_end * V_end -   V_max *   V_max) / (2.0f * Sde);
	
	S = ABS(pos - P_start);
		
	// 规划RPM
	if     (S < Sac)       TURN_MOTOR_TARGET_RPM = sqrt(2.0f * Aac * S + V_start * V_start);         // 加速阶段
	else if(S < (Sac+Sco)) TURN_MOTOR_TARGET_RPM = V_max;                                            // 匀速阶段
	else                   TURN_MOTOR_TARGET_RPM = sqrt(V_end * V_end - 2.0f * Ade * ABS(Ssu - S));  // 减速阶段
	
	// 分配合适的正负号
	if(P_end < P_start) TURN_MOTOR_TARGET_RPM = -TURN_MOTOR_TARGET_RPM;
	
	if(Ssu - S <=2)
	{
		TURN_MOTOR_TARGET_RPM = 0;
		ROBOT.TURNSTATE = TURN_SUCCEED;
	}
}

int DELAY_ms(int time_delay)
{
	static float time_first;
	static int count_init = 0;

	if(count_init == 0)
	{
		  time_first = Get_OSTime();
			count_init = 1;
	}
	if( (Get_OSTime() - time_first) > time_delay )
	{
		  count_init = 0;
		  return 1;
	}
	else
	{
		  return 0;
	}

}