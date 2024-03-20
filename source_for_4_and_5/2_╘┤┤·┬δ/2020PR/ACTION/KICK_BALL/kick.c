#include "main.h"

void kick_action_task(void)
{
  static u8 flow_counter = 0;
	static u32 time_10ms_counter = 0;
	
	if(KICK_STATE == KICK_START)
	{
		switch(flow_counter)
		{
			case 0:
			{

				cylinder_turn_on(HOLD_FOOT);
				
				if(time_10ms_counter < PREPARE_KICK_DELAY){time_10ms_counter ++; return;}

				cylinder_turn_on(CATCH_BALL_2ST);
				
				cylinder_turn_on(SHOOT_PULL);
				
				time_10ms_counter = 0;
				flow_counter ++;
			}
			case 1:
			{
				if(time_10ms_counter < RESET_KICK_DELAY){time_10ms_counter ++; return;}
				
        cylinder_turn_off(SHOOT_PULL);
				
        UNDER_ARM_NOW_MOTION = &UNDER_ARM_PICK_MOTION;

				time_10ms_counter = 0;
				flow_counter ++;
			}break;
			
			case 2:
			{
				if(time_10ms_counter < WAIT_KICK_DELAY){time_10ms_counter ++; return;}

				cylinder_turn_off(CATCH_BALL_2ST);
				
				time_10ms_counter = 0;
				flow_counter ++;
			}break;

			case 3:
			{
				if(time_10ms_counter < WAIT_KICK_DELAY){time_10ms_counter ++; return;}

        UNDER_ARM_NOW_MOTION = &UNDER_ARM_PUT_MOTION;
				
				if(ls_read_limit_switch_status(UNDER_ARM_RESET_LIMIT_SWITCH) == SET) return;
				
				M3508_UNDER_ARM_MOTOR_REAL_INFO.REAL_ANGLE = UNDER_ARM_TURN_INIT_POS;  /*WAITING_DEBUG*/
				
				UNDER_ARM_NOW_MOTION = &UNDER_ARM_HOLD_MOTION;
				
				
				time_10ms_counter = 0;
				flow_counter ++;
			}break;
			
			case 4:
			{
				if(time_10ms_counter < WAIT_KICK_DELAY){time_10ms_counter ++; return;}
				
        cylinder_turn_off(HOLD_FOOT);
				
				time_10ms_counter = 0;
				flow_counter ++;
			}break;
			
			case 5:
			{
				if(time_10ms_counter < WAIT_KICK_DELAY){time_10ms_counter ++; return;}
				

				
				time_10ms_counter = 0;
				flow_counter ++;
			}break;
			
			default:
			{
				KICK_STATE = KICK_SUCCEEDED;
				flow_counter = 0;
			}
		}
	}
	else
	{
		
	}

	
}
