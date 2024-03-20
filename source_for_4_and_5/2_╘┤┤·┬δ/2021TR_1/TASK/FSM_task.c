#include "includes.h"


ROBOT_STATE_ITEMS ROBOT_STATE = ROBOT_STATE_INIT;

float v_target = 2.5;
float a_target = 0.01;

float yaw_des;
float pitch_des;
void FSM_task(void *pvParameters)
{
	int flag = 0;								// ÂË²¨
	int press_time_counter = 0;	// ³¤°´
	
	while(1)
	{
		PS2_update_ps2_command();
		
		switch(ROBOT_STATE)
		{
			/* ÅÜÂ·¾¶ */
			case ROBOT_STATE_AUTO:
				break;
			
			/* ³õÊ¼»¯ */
			case ROBOT_STATE_INIT:
				CAST_STATE = CAST_STATE_HOLD;
				TAKE_STATE = TAKE_INIT;
				MOVE_STATE = MOVE_STOP;
				if(PS2_KEY)
				{
					switch(PS2_KEY)
					{
						case PSB_R2:
							press_time_counter++;
							if (press_time_counter > 15)
							{
//								Cylinder_4_ON();
//								vTaskDelay(400);
//								Cylinder_4_OFF();
//								vTaskDelay(1800);
//								Cylinder_3_ON();
//								vTaskDelay(400);
//								Cylinder_3_OFF();
								press_time_counter = 0;
//								vTaskDelay(1000);vTaskDelay(1000);vTaskDelay(1000);
//								ROBOT_STATE = ROBOT_STATE_RELOAD;
								MOVE_STATE = MOVE_2_LOAD_ARROW;
								ROBOT_STATE = ROBOT_STATE_AUTO;
							}
							break;
						
						case PSB_L2:
							press_time_counter++;
							if (press_time_counter > 15)
							{
								press_time_counter = 0;
								ROBOT_STATE = ROBOT_STATE_RELOAD;
							}
							break;
							
						default:
							break;
					}
				}
				else press_time_counter = 0;
				break;
			
			/* È¡¼ý×´Ì¬ */
			case ROBOT_STATE_RELOAD:
				if(PS2_KEY)
				{
					switch(PS2_KEY)
					{
						// ÅÜÈ¥Éä¼ýµã1
						case PSB_L3:
							if ((ROBOT_REAL_POS_DATA.POS_X+10600)*(ROBOT_REAL_POS_DATA.POS_X+10600) < 250000)
							{
//								TAKE_STATE = TAKE_INIT;
//								vTaskDelay(500);
//								X1[0] = ROBOT_REAL_POS_DATA.POS_X;
//								Y1[0] = ROBOT_REAL_POS_DATA.POS_Y;
								MOVE_STATE = MOVE_2_CAST_POINT_1;
								ROBOT_STATE = ROBOT_STATE_AUTO;
							}
							break;
						
						// ×°Ìî×óÉä¼ý»ú¹¹ 
						case PSB_L2:
							if (TAKE_STATE == TAKE_DONE || CLAW_STATE == CLAW_STATE_CLOSE)
							{
								TAKE_STATE = TAKE_LEFT;
								CLAW_STATE = CLAW_STATE_LOCK;
							}
							break;
						
						// ×°ÌîÓÒÉä¼ý»ú¹¹
						case PSB_R2:
							if (TAKE_STATE == TAKE_DONE || CLAW_STATE == CLAW_STATE_CLOSE)
							{
								TAKE_STATE = TAKE_RIGHT;
								CLAW_STATE = CLAW_STATE_LOCK;
							}
							break;
						
						// µ×ÅÌÄæÊ±ÕëÐý×ª
						case PSB_L1:
							ROBOT_TARGET_VELOCITY_DATA.W_RPM = -0.3;
							break;
						
						// µ×ÅÌË³Ê±ÕëÐý×ª
						case PSB_R1:
							ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0.3;
							break;
						
						// µ×ÅÌÇ°½ø
						case PSB_PAD_UP:
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM += a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM > v_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = v_target;
							break;
						
						// µ×ÅÌºóÍË
						case PSB_PAD_DOWN:
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM -= a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM < -v_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = -v_target;
							break;
						
						// µ×ÅÌ×óÒÆ
						case PSB_PAD_LEFT:
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM -= a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM < -v_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -v_target;
							break;
						
						// µ×ÅÌÓÒÒÆ
						case PSB_PAD_RIGHT:
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM += a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM > v_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = v_target;
							break;
						
						// Ì§ÉýÉÏÉý
						case PSB_GREEN:
							if (!flag)
							{
								if (CAST_STATE == CAST_STATE_BOTTOM)
								{
									CAST_STATE = CAST_STATE_HOLD;
									flag = 1;
								}
								else if (CAST_STATE == CAST_STATE_HOLD)
								{
									CAST_STATE = CAST_STATE_TOP;
									flag = 1;
								}
								else
								{
									if (cast_target_v < CAST_V_MAX) cast_target_v += CAST_ACC;
								}
							}
							break;
						
						// Ì§ÉýÏÂ½µ
						case PSB_BLUE:
							if (!flag)
							{
								if (CAST_STATE == CAST_STATE_TOP)
								{
									CAST_STATE = CAST_STATE_HOLD;
									flag = 1;
								}
								else if (CAST_STATE == CAST_STATE_HOLD)
								{
									CAST_STATE = CAST_STATE_BOTTOM;
									flag = 1;
								}
								else
								{
									if (cast_target_v < CAST_V_MAX) cast_target_v += CAST_ACC;
								}
							}
							break;
						
						// È¡¼ý¸´Î»
						case PSB_PINK:
							if (!flag)
							{
								if (TAKE_STATE == TAKE_INIT)
								{
									TAKE_STATE = TAKE_DONE;
									flag = 1;
								}
								else if (TAKE_STATE == TAKE_LEFT)
								{
									if (CLAW_STATE == CLAW_STATE_CLOSE)
									{
										CLAW_STATE = CLAW_STATE_OPEN;vTaskDelay(200);
										Throwing_Institution_Control(LEFT_TI, 15.0f, 0, RPM_MIN, UNSET);vTaskDelay(500);
										TAKE_STATE = TAKE_RESET;
										CAST_STATE = CAST_STATE_BOTTOM;
									}
								}
								else if (TAKE_STATE == TAKE_RIGHT)
								{
									if (CLAW_STATE == CLAW_STATE_CLOSE)
									{
										CLAW_STATE = CLAW_STATE_OPEN;vTaskDelay(200);
										Throwing_Institution_Control(RIGHT_TI, -15.0f, 0, RPM_MIN, UNSET);vTaskDelay(500);
										TAKE_STATE = TAKE_RESET;
										CAST_STATE = CAST_STATE_BOTTOM;
									}
								}
								else if (TAKE_STATE == TAKE_DONE)
								{
									press_time_counter++;
									if (press_time_counter > 15 )
									{
										TAKE_STATE = TAKE_INIT;
										flag = 1;
									}
								}
							}
							
							break;
						
						// ºÏ×¦¡¢ËÉ×¦ 
						case PSB_RED:
							if (!flag)
							{
								if (CLAW_STATE == CLAW_STATE_CLOSE)
								{
									press_time_counter++;
									if (press_time_counter > 15 )
									{
										CLAW_STATE = CLAW_STATE_OPEN;
										flag = 1;
									}
								}
								else if (CLAW_STATE == CLAW_STATE_OPEN)
								{
									CLAW_STATE = CLAW_STATE_CLOSE;
									flag = 1;
								}
							}
							break;
										
						default:
							break;
					}
				}
				else 
				{
					if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM > 0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM -= a_target;
					else if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM < -0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM += a_target;
					
					if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM > 0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM -= a_target;
					else if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM < -0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM += a_target;
					
					if (cast_target_v > CAST_V_MIN) cast_target_v -= CAST_ACC;
					
					ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;
					
					flag = 0;
					
					press_time_counter = 0;
				}
				break;
				
			/* Éä¼ý×´Ì¬ */
			case ROBOT_STATE_THROW:
				if(PS2_KEY)
				{
					switch(PS2_KEY)
					{
						// È¡¼ý»ú¹¹ÕÅ¿ª
						case PSB_R3:
							TAKE_STATE = TAKE_DONE;
							break;
						
						// Ä¦²ÁÂÖ¼õËÙ
						case PSB_L1:
							if (!flag)
							{
								CTRL_DES.rpm_des -= 20;
								flag = 1;
							}
							break;
							
						// Ä¦²ÁÂÖ¼ÓËÙ
						case PSB_R1:
							if (!flag)
							{
								CTRL_DES.rpm_des += 20;
								flag = 1;
							}
							break;
						
						// ·¢Éä
						case PSB_R2:
							if (!flag)
							{
								Throwing_Institution_Control(LEFT_TI, CTRL_DES.yaw_des, CTRL_DES.pitch_des, CTRL_DES.rpm_des, SET);
								flag = 1;
							}
							break;
						 
							 
						// ¸©Ñö+
						case PSB_PAD_UP:
							if (!flag)
							{
								CTRL_DES.pitch_des += 1.5f;
								flag = 1;
							}
							break;
							
						
						// ¸©Ñö-
						case PSB_PAD_DOWN:
							if (!flag)
							{
								CTRL_DES.pitch_des -= 1.5f;
								flag = 1;
							}
							break;
						
						// Æ«º½-
						case PSB_PAD_LEFT:
							if (!flag)
							{
								CTRL_DES.yaw_des -= 1.5f;
								flag = 1;
							}
							break;
						
						// Æ«º½+
						case PSB_PAD_RIGHT:
							if (!flag)
							{
								CTRL_DES.yaw_des += 1.5f;
								flag = 1;
							}
							break;
							
						// ÏÂ½µ
						case PSB_BLUE:
							if (!flag)
							{
								if (CAST_STATE == CAST_STATE_TOP)
								{
									CAST_STATE = CAST_STATE_HOLD;
									flag = 1;
								}
								else if (CAST_STATE == CAST_STATE_HOLD)
								{
									CAST_STATE = CAST_STATE_BOTTOM;
									flag = 1;
								}
								else
								{
									if (cast_target_v < CAST_V_MAX) cast_target_v += CAST_ACC;
								}
							}
							break;
						
						// ÉÏÉý
						case PSB_GREEN:
							if (!flag)
							{
								if (CAST_STATE == CAST_STATE_BOTTOM) CAST_STATE = CAST_STATE_HOLD;
								else CAST_STATE = CAST_STATE_TOP;
								flag = 1;
							}
							break;
							
						// ¸´Î»
						case PSB_PINK:
							if (!flag)
							{
								press_time_counter++;
								if (press_time_counter > 15)
								{
									CTRL_DES = CTRL_RESET;
									Throwing_Institution_Control(RESET_TI, 0, 0, 0, 0);
									flag = 1;
								}
							}
							break;
						
						default:
							break;
					}
				}
				else
				{
					Throwing_Institution_Control(LEFT_TI, CTRL_DES.yaw_des, CTRL_DES.pitch_des, CTRL_DES.rpm_des, UNSET);
					if (cast_target_v > CAST_V_MIN) cast_target_v -= CAST_ACC;
					flag = 0;
					press_time_counter = 0;
				}
				break;

			/* debug×´Ì¬ */
			case ROBOT_STATE_DEBUG:
				if(PS2_KEY)
				{
					switch(PS2_KEY)
					{
						// µ×ÅÌÄæÊ±ÕëÐý×ª
						case PSB_L1:
							ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0.5;
							break;
						
						case PSB_L2:
							break;
						
						// µ×ÅÌË³Ê±ÕëÐý×ª
						case PSB_R1:
							ROBOT_TARGET_VELOCITY_DATA.W_RPM = -0.5;
							break;
						
						case PSB_R2:
							break;
						
						// µ×ÅÌÇ°½ø
						case PSB_PAD_UP:
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM += a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM > v_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = v_target;
							break;
						
						// µ×ÅÌºóÍË
						case PSB_PAD_DOWN:
							ROBOT_TARGET_VELOCITY_DATA.Vx_RPM -= a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM < -v_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = -v_target;
							break;
						
						// µ×ÅÌ×óÒÆ
						case PSB_PAD_LEFT:
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM -= a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM < -v_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = -v_target;
							break;
						
						// µ×ÅÌÓÒÒÆ
						case PSB_PAD_RIGHT:
							ROBOT_TARGET_VELOCITY_DATA.Vy_RPM += a_target;
							if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM > v_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = v_target;
							break;
						
						// Ì§ÉýÉÏÉý
						case PSB_GREEN:
							if (!flag)
							{
								CAST_STATE = CAST_STATE_TOP;
								flag = 1;
							}
							break;
						
						// Ì§ÉýÏÂ½µ
						case PSB_BLUE:
							if (!flag)
							{
								CAST_STATE = CAST_STATE_BOTTOM;
								flag = 1;
							}
							break;
										
						default:
							break;
					}
				}
				else 
				{
//					if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM > 0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM -= a_target;
//					else if(ROBOT_TARGET_VELOCITY_DATA.Vy_RPM < -0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vy_RPM += a_target;
//					
//					if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM > 0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM -= a_target;
//					else if(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM < -0.01f*a_target) ROBOT_TARGET_VELOCITY_DATA.Vx_RPM += a_target;
//					if (CAST_STATE == CAST_STATE_HOLD)
//					{
//						if(CAST_MOTOR_TARGET_RPM > 0.01f*a_target) CAST_MOTOR_TARGET_RPM -= a_target;
//						else if(CAST_MOTOR_TARGET_RPM < -0.01f*a_target) CAST_MOTOR_TARGET_RPM += a_target;
//					}
					flag = 0;
					
					ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;
				}
				break;
			
			default:
				break;
		}
		
		vTaskDelay(10);
	}
}


