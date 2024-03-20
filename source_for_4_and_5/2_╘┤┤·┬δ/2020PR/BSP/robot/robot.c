#include "includes.h"


ROBOT_STATE_ITEMS ROBOT = {0};

float INIT_POS;
float INIT_X;
float INIT_Y;
void FSM_RUN(void *pdata)
{
	while(1)
	{
		if(PS2_KEY == 1)
		{
			ROBOT.NOW_STATE = ROMOTE_RESTART;
//			robot.x = 0;
//			ROBOT_REAL_POS_DATA.Y_POS = 0;
//			TURN_MOTOR_TARGET_RPM = 0;
//			OFFSET_YAW = ROBOT_REAL_POS_DATA.YAW_POS;
			INIT_X = ROBOT_REAL_POS_DATA.X_POS;
			INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
			ROBOT.MOVE_STATE = STOP;
			TIM_Cmd(TIM12, DISABLE);		 
		}
	  switch(ROBOT.NOW_STATE)
	  {
		   case ROBOT_INIT:
		   {		
				 	TIM12->CCR1 = 0;
			    TIM12->CCR2 = 0;
				  OSTimeDly(100);
				 	cylinder_off(CATCH_BALL_1ST);
					OSTimeDly(300);
				 	OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
					OSTimeDly(1000);
				 	OSTimeDly(1000);
				 	OSTimeDly(1000);
				  ROBOT.SHOOTSTATE = THROW_START;	
			  	if(Get_KEY(KICK_KEY))
					{
						  TURN_MOTOR_TARGET_RPM = 0;
						  ROBOT.SHOOTSTATE = THROW_START;
						  ROBOT.NOW_STATE = KICK_2nd;
						  ROBOT.MOVE_STATE = STOP;
					}
					else if(Get_KEY(KICK_START_KEY))
					{
						  TURN_MOTOR_TARGET_RPM = 0;
						  ROBOT.SHOOTSTATE = THROW_START;
						  ROBOT.NOW_STATE = KICK_1st;
						  ROBOT.MOVE_STATE = STOP;					
					}
					else
					{

          TIM12->CCR1 = 1300;
					TIM12->CCR2 = 1300;
					OSTimeDly(1000);
					TIM12->CCR1 = 1155;
					TIM12->CCR2 = 1155;				 
			    ROBOT.NOW_STATE = ROMOTE_PREPARATION;
					}
		   }
		   break;	 
			 
			 case ROMOTE_RESTART:
			 {	        
					TURN_MOTOR_TARGET_RPM = 0;
					ROBOT.SHOOTSTATE = THROW_START;
					OSTimeDly(1000);
					Launch_Motor_init(20000-1,83);
					OSTimeDly(1000);
          
          OSTimeDly(1000);
					TIM12->CCR1 = 0;
					TIM12->CCR2 = 0;
				 
					OSTimeDly(500);
					TIM12->CCR1 = 1300;
					TIM12->CCR2 = 1300;
					OSTimeDly(500);

					TIM12->CCR1 = 1155;
					TIM12->CCR2 = 1155;	
					ROBOT.NOW_STATE = ROMOTE_PREPARATION;	  
			 }
		   break;		
			 
		   case ROMOTE_PREPARATION:
		   {
          TURN_MOTOR_TARGET_RPM = 0;
		      if(Get_KEY(START_KEY))
		      {
							AK80_motion_control(0x01,-0.0f*PI, 0, 10, 1, 0);
					    AK80_motion_control(0x02,-0.0f*PI, 0, 10, 1, 0);
				      AK80_motion_control(0x03,-0.0f*PI, 0, 10, 1, 0);
              ROBOT.NOW_STATE  = START_TO_BALL_1st;
				      ROBOT.MOVE_STATE = MOVING;
						  ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
						  INIT_X = ROBOT_REAL_POS_DATA.X_POS;
						  INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
		      }
					
					if(Get_KEY(RESTART_TO_1_KEY))
					{
							TIM12->CCR1 = 1165;
							TIM12->CCR2 = 1165;
							ROBOT.NOW_STATE  = RESTART_TO_2;
							ROBOT.MOVE_STATE = MOVING;
							
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
							INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				 
					}
					if(Get_KEY(RESTART_TO_2_KEY))
		      {
							TIM12->CCR1 = 1190;
							TIM12->CCR2 = 1190;
							ROBOT.NOW_STATE  = RESTART_TO_3;
							ROBOT.MOVE_STATE = MOVING;
							
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
							INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				 
		      }
					
					if(Get_KEY(RESTART_TO_3_KEY))
		      {
							TIM12->CCR1 = 1165;
							TIM12->CCR2 = 1165;
							ROBOT.NOW_STATE  = RESTART_TO_4;
							ROBOT.MOVE_STATE = MOVING;

							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
							INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				 
		      }
					if(Get_KEY(TO_5th_ball))
		      {
							TIM12->CCR1 = 1165;
							TIM12->CCR2 = 1165;
							ROBOT.NOW_STATE  = TO_BALL_5th;
							ROBOT.MOVE_STATE = MOVING;

							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
							INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				 
		      }
					if(Get_KEY(KICK_START_KEY))
					{
						  TIM12->CCR1 = 0;
			        TIM12->CCR2 = 0;
						  ROBOT.NOW_STATE = KICK_1st;
						  ROBOT.MOVE_STATE = STOP;
					}
					if(Get_KEY(KICK_KEY))
					{
						  TIM12->CCR1 = 0;
			        TIM12->CCR2 = 0;
						  ROBOT.NOW_STATE = KICK_2nd;
						  ROBOT.MOVE_STATE = STOP;
					}
			 }
			 break;
		
		   case ROMOTE_START:
			 {
	
			 }
			 break;
		
			 case START_TO_BALL_1st:
			 { 			 
				  if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
				  {
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
							TIM12->CCR1 = 1160;
							TIM12->CCR2 = 1160;
							cylinder_off(CATCH_BALL_1ST);
							
							OSTimeDly(80);
							ROBOT.NOW_STATE = THROW_1st_BALL;//进行夹球动作

							ROBOT.SHOOTSTATE = THROW_START;		
					}
					else
					{
			 		TURNPLATE_adjusting(-47.3);
					}
			 }
			 break;
		
			 case THROW_1st_BALL:
			 {
				  if(ROBOT.SHOOTSTATE == THROW_SUCCEEDED)
				  { 
						  TIM12->CCR1 = 1174;
							TIM12->CCR2 = 1174;
							ROBOT.NOW_STATE = TO_BALL_2nd;
							ROBOT.MOVE_STATE = MOVING;
						  OSTimeDly(100);
							INIT_POS = robot.angle;
						  ROBOT.TURNSTATE = TURN_STOP;
						  INIT_X = ROBOT_REAL_POS_DATA.X_POS;
						  INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				  }	
			 }
			 break;
		
			 case TO_BALL_2nd:
			 { 
				  
				  if(ROBOT.MOVE_STATE == ARRIVE )//到达位置
				  {
							TIM12->CCR1 = 1174;
							TIM12->CCR2 = 1174;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_2nd_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
								  OSTimeDly(100);
									//OSTimeDly(500);
									ROBOT.SHOOTSTATE = THROW_START;
							}
				  }
					else
					{
					    TURNPLATE_adjusting(-47.3);  
					}
			 }
			 break;
			
			 case THROW_2nd_BALL:
			 {
				  if(ROBOT.SHOOTSTATE == THROW_SUCCEEDED)//夹球完毕
				  {
						  TIM12->CCR1 = 1193;
							TIM12->CCR2 = 1193;
							ROBOT.NOW_STATE = TO_BALL_3rd;//去第三个夹球区
							ROBOT.MOVE_STATE = MOVING;
						  OSTimeDly(100);
							INIT_POS = robot.angle;
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
						  INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				  }		
						
			 }	
			 break;
		
		
			 case TO_BALL_3rd:
			 {
				  
				  if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
				  {
				  	  TIM12->CCR1 = 1194;
							TIM12->CCR2 = 1194;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_3rd_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
								  OSTimeDly(100);
									//OSTimeDly(500);
									ROBOT.SHOOTSTATE = THROW_START;	
							}
				  }
					else
					{
					    TURNPLATE_adjusting(-47.3);
					}							
			 }
			 break;
			 
			 
	
			 case THROW_3rd_BALL:
			 {
				  if(ROBOT.SHOOTSTATE == THROW_SUCCEEDED)
				  {
							TIM12->CCR1 = 1169;
							TIM12->CCR2 = 1169;
							ROBOT.NOW_STATE = TO_BALL_4th;
							ROBOT.MOVE_STATE = MOVING;
						  OSTimeDly(100);
							INIT_POS = robot.angle;
							INIT_X = ROBOT_REAL_POS_DATA.X_POS;
						  INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
				  }	

			 }
			 break;
		
			 case TO_BALL_4th:
			 {
					if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
					{
							TIM12->CCR1 = 1171;
							TIM12->CCR2 = 1171;
						  ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_4th_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
									OSTimeDly(100);
									ROBOT.SHOOTSTATE = THROW_START;		
							}
			    }
					else
					{
					    TURNPLATE_adjusting(-50.3);
					}

		   }
	     break;
		
		   case THROW_4th_BALL:
		   {
			    if(ROBOT.SHOOTSTATE == THROW_SUCCEEDED)
			    {
							TIM12->CCR1 = 0;
							TIM12->CCR2 = 0;
						  ROBOT.NOW_STATE = TO_START;
						  ROBOT.MOVE_STATE = MOVING;
				      OSTimeDly(100);
						  INIT_POS = robot.angle;
				 		  INIT_X = ROBOT_REAL_POS_DATA.X_POS;
						  INIT_Y = ROBOT_REAL_POS_DATA.Y_POS;
//						  ROBOT.KICK_SAFE=no_kick;
//              cylinder_off(CATCH_SEAT_1);

//				      cylinder_off(CATCH_SEAT_2);
//				      ROBOT.KICK_STATE = KICK_END3;
//			 	      ROBOT.KICK_SAFE=no_kick;

			    }	

			 }
			 break;
		
			 case TO_BALL_5th:
			 {			 
				  if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
				  {	 
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
					    TIM12->CCR1 = 1172;
							TIM12->CCR2 = 1172;
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_5th_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
									OSTimeDly(100);
									ROBOT.SHOOTSTATE = THROW_START;	
							}	
					}
					else
					{
					    TURNPLATE_adjusting(-47.3);
					}
			 }
			 break;
		
			 case THROW_5th_BALL:
			 {
					if(ROBOT.SHOOTSTATE == THROW_SUCCEEDED)
					{
							TIM12->CCR1 = 0;
							TIM12->CCR2 = 0;
						  ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
							ROBOT.NOW_STATE = BALL_5th_TO_START;
							ROBOT.MOVE_STATE = MOVING;
				
					}
		
			 }
			 break;
		
			 
			
			 case TO_START:
			 {
				  
					if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
					{
							TIM12->CCR1 = 0;
			        TIM12->CCR2 = 0;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
							ROBOT.SHOOTSTATE = THROW_STOP;
							ROBOT.NOW_STATE = KICK_1st;
						  TURN_MOTOR_TARGET_RPM = 0;
							cylinder_off(CATCH_SEAT_3);
              						
					}
					
					    TURNPLATE_adjusting(0);
					
			 }
			 break;
			 
			 case BALL_5th_TO_START:
			 {
				  
					if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
					{
							TIM12->CCR1 = 0;
			        TIM12->CCR2 = 0;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM  = 0;
							ROBOT.SHOOTSTATE = THROW_STOP;
							ROBOT.NOW_STATE = KICK_1st;
						  TURN_MOTOR_TARGET_RPM = 0;
							cylinder_off(CATCH_SEAT_3);
              						
					}
					
					    TURNPLATE_adjusting(0);
					
			 }
			 break;
			 
		 	 case RESTART_TO_2:
			 { 
				  
				  if(ROBOT.MOVE_STATE == ARRIVE )//到达位置
				  {
							TIM12->CCR1 = 1170;
							TIM12->CCR2 = 1170;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_2nd_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
									OSTimeDly(200);
									ROBOT.SHOOTSTATE = THROW_START;
							}
				  }
					else
					{
					    TURNPLATE_adjusting(-46.1);  
					}
			 }
			 break;
			 
			 	 case RESTART_TO_3:
			 {
				  
				  if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
				  {
				  	  TIM12->CCR1 = 1192;
							TIM12->CCR2 = 1192;
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_3rd_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
								  OSTimeDly(200);
									//OSTimeDly(500);
									ROBOT.SHOOTSTATE = THROW_START;	
							}
				  }
					else
					{
					    TURNPLATE_adjusting(-47.3);
					}		

					
			 }
			 break;
			 case RESTART_TO_4:
			 {
					if(ROBOT.MOVE_STATE == ARRIVE)//到达位置
					{
							TIM12->CCR1 = 1165;
							TIM12->CCR2 = 1165;
						  ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;//停车
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
							ROBOT_TARGET_VELOCITY_DATA.W_RPM 	= 0;
        
							OSTimeDly(100);
							ROBOT.SHOOTSTATE = THROW_RESET;//手臂复位
							OSTimeDly(100);
							cylinder_on(CATCH_BALL_1ST);//张开夹爪
							if(ROBOT.SHOOTSTATE == THROW_RESET_SUCCEEDED)//手臂复位完
							{
									ROBOT.NOW_STATE = THROW_4th_BALL;//进行夹球动作
									cylinder_off(CATCH_BALL_1ST);
									OSTimeDly(200);
									ROBOT.SHOOTSTATE = THROW_START;		
							}
			    }
					else
					{
					    TURNPLATE_adjusting(-46.5);
					}

		   }
	     break;
			 case KICK_1st:
			 {
					if(Get_KEY(KICK_KEY))
					{
						 TIM12->CCR1 = 0;
			       TIM12->CCR2 = 0;
						 ROBOT.NOW_STATE = KICK_2nd;
						 ROBOT.MOVE_STATE = STOP;
					}
           TURN_MOTOR_TARGET_RPM = 0;

			 }
			 break;
			 
			 case KICK_2nd:
			 {
				 	if(Get_KEY(KICK_START_KEY))
					{
						 TIM12->CCR1 = 0;
			       TIM12->CCR2 = 0;
						 ROBOT.NOW_STATE = KICK_1st;
						 ROBOT.MOVE_STATE = STOP;
					}
           
//				    TURN_MOTOR_TARGET_RPM = 0;
			 }
			 break;

			 default:break;
		}

		OSTimeDly(5);
  }
}



