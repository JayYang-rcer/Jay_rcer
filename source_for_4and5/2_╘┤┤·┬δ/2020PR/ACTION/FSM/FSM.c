#include "includes.h"


// 初始化各个状态值
ROBOT_STATE_ITEMS NOW_STATE = ROMOTE_PREPARATION;   //初始状态为准备状态
       
MOVING_STATE_ITEMS MOVING_STATE = MOVING_STOP;      //移动模式为移动状态关闭

CATCH_STATE_ITEMS CATCH_STATE = CATCH_STOP;         //夹球模式为夹球状态关闭

THROW_STATE_ITEMS THROW_STATE = THROW_STOP;         //传球模式为传球状态关闭

KICK_STATE_ITEMS KICK_STATE = KICK_STOP;            //踢球模式为踢球状态关闭

ROMOTE_STATE_ITEMS ROMOTE_STATE = ROMOTE_START;     // 初始时允许遥控

OPTAINAL_MODE_ITEMS OPTAINAL_MODE = MODE_UNKNOWN;   //选择模式为未知状态

TURN_RESET_STATE_ITEMS TURN_RESET_STATE = TURN_RESET_START;  // 上转盘开启复位

ARM_RESET_STATE_ITEMS ARM_RESET_STATE = ARM_RESET_STOP;        // 机械夹爪开启复位


// 有限状态机
void FSM_RUN(void *pdata)
{ 
	while(1)
	{
	switch(NOW_STATE)
	{		
		// 通过遥控选择模式
		case ROMOTE_PREPARATION:         //遥控模式下可选择的路径
		{		
			  MOVING_STATE   = MOVING_GOING;  //移动模式开启
				CATCH_STATE    = CATCH_STOP;   //夹球模式关闭
				THROW_STATE    = THROW_STOP;   //传球模式关闭
			  KICK_STATE     = KICK_STOP;    //踢球模式关闭
			  NOW_STATE = START_TO_BALL_1st;  
			  ROMOTE_STATE = ROMOTE_STOP; 
			  MOVING_STATE = MOVING_GOING;

		}
		break;
		
		case START_TO_BALL_1st:    //从启动区前往夹球点1
		{

			// 如果到达夹球点1
			if((MOVING_STATE == MOVING_ARRIVED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(ARM_RESET_LIMIT_SWITCH) == RESET))
			{
				TURN_RESET_STATE = TURN_RESET_STOP;  //复位转盘开启
				MOVING_STATE    = MOVING_STOP;        //移动模式关闭
				CATCH_STATE     = CATCH_START;        //夹球模式开启
				THROW_STATE     = THROW_STOP;         //传球模式关闭
				KICK_STATE      = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE    = ROMOTE_STOP;        //遥控模式关闭
        NOW_STATE       = CATCH_1st_BALL;       //当前状态为抓取第1个一类球
			}
		}
		break;
		
			case CATCH_1st_BALL:     //抓取第1个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED) 				
			{
				TURN_RESET_STATE = TURN_RESET_STOP;  //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE    = MOVING_STOP;       //移动模式关闭
				CATCH_STATE     = CATCH_STOP;        //夹球模式关闭
				THROW_STATE     = THROW_START;       //传球模式开启
				KICK_STATE      = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE    = ROMOTE_START;      //遥控模式开启
        NOW_STATE       = THROW_1st_BALL;       //当前状态为传第1个一类球
			}
		}
		break;
		
		case THROW_1st_BALL:      //传第1个一类球
		{
			//如果传球成功
			if(THROW_STATE == THROW_SUCCEEDED)
			{
				TURN_RESET_STATE = TURN_RESET_START; //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_STOP;    //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;       //移动模式开启
				CATCH_STATE    = CATCH_START;         //夹球模式开启
				THROW_STATE    = THROW_STOP;       //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;       //遥控模式关闭
        NOW_STATE      = TO_BALL_2nd;  //当前状态为从夹球点1到夹球点2
			}
		}
		break;
		
		case TO_BALL_2nd:    //从夹球点1到夹球点2
		{
			// 如果到达夹取点2
			if((MOVING_STATE == MOVING_ARRIVED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(PULL_RESET_LIMIT_SWITCH) == RESET))
			{
				TURN_RESET_STATE = TURN_RESET_START;  //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_STOP;   //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_START;       //夹球模式开启
				THROW_STATE    = THROW_STOP;        //传球模式关闭
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;       //遥控模式关闭
        NOW_STATE      = CATCH_2nd_BALL;       //当前状态为抓取第2个一类球
			}
		}
		break;
		
	  case CATCH_2nd_BALL:       //抓取第2个一类球
		{
	    //如果夹球成功
			if((CATCH_STATE == CATCH_SUCCEEDED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(PULL_RESET_LIMIT_SWITCH) == RESET))   
			{
				TURN_RESET_STATE = TURN_RESET_STOP;  //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE   = MOVING_STOP;        //移动模式关闭
				CATCH_STATE    = CATCH_STOP;         //夹球模式关闭
				THROW_STATE    = THROW_START;        //传球模式开启
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
        NOW_STATE      = THROW_2nd_BALL;  //当前状态为传第2个一类球
			}
		}
		break;
		
		case THROW_2nd_BALL:     //传第2个一类球   
		{
			// 如果传球成功
			if((THROW_STATE == THROW_SUCCEEDED)&&(ARM_RESET_STATE == ARM_RESET_SUCCEEDED))
			{
				TURN_RESET_STATE = TURN_RESET_START;  //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_START;     //复位机械臂开始
				MOVING_STATE   = MOVING_GOING;        //移动模式开启
				CATCH_STATE    = CATCH_STOP;          //夹球模式开启
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;        //遥控模式关闭
        NOW_STATE      = TO_BALL_3rd;//当前状态为从夹球点2到夹球点3
			}
		}
		break;
		
    case TO_BALL_3rd://从夹球点2到夹球点3
		{
			// 如果到达夹球点3
			if((MOVING_STATE == MOVING_ARRIVED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(PULL_RESET_LIMIT_SWITCH) == RESET))
			{
				TURN_RESET_STATE = TURN_RESET_START; //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;    //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;        //移动模式关闭
//				MOVING_STATE   = MOVING_GOING;     //移动模式开启
				CATCH_STATE    = CATCH_START;        //夹球模式开启
				THROW_STATE    = THROW_STOP;         //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
				NOW_STATE      = CATCH_3rd_BALL;     //抓取第3个一类球
			}
		}
		break; 
		
		case CATCH_3rd_BALL:                     //当前状态为抓取第1个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED)   
			{
				TURN_RESET_STATE = TURN_RESET_STOP;  //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE   = MOVING_STOP;        //移动模式关闭
				CATCH_STATE    = CATCH_STOP;         //夹球模式开启
				THROW_STATE    = THROW_START;        //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
        NOW_STATE      = BALL_3st_TO_PASS_3rd;  //当前状态为传第3个一类球
			}
		}
		break;
		
		case BALL_3st_TO_PASS_3rd:                 //当前状态为抓取第1个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED)   
			{
				TURN_RESET_STATE = TURN_RESET_STOP; //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE   = MOVING_STOP;        //移动模式关闭
				CATCH_STATE    = CATCH_STOP;         //夹球模式开启
				THROW_STATE    = THROW_START;        //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
        NOW_STATE      = THROW_3rd_BALL;  //当前状态为传第3个一类球
			}
		}
		break;
		
		case THROW_3rd_BALL:     //传第3个一类球
		{
			// 如果传球成功
			if((THROW_STATE == THROW_SUCCEEDED)&&(ARM_RESET_STATE == ARM_RESET_SUCCEEDED))
			{	
				TURN_RESET_STATE = TURN_RESET_START; //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_START;    //复位机械臂关闭				
				MOVING_STATE   = MOVING_GOING;       //移动模式开启
				CATCH_STATE    = CATCH_STOP;         //夹球模式开启
				THROW_STATE    = THROW_STOP;         //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
        NOW_STATE      = TO_BALL_4th;//当前状态为从夹球点3到夹球点4
			}
		}
		break;
		
		case TO_BALL_4th://从夹球点3到夹球点4
		{
			// 如果到达夹球点4
			if((MOVING_STATE == MOVING_ARRIVED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(PULL_RESET_LIMIT_SWITCH) == RESET))
			{			
				TURN_RESET_STATE = TURN_RESET_START;  //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;    //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;        //移动模式关闭
				CATCH_STATE    = CATCH_START;        //夹球模式开启
				THROW_STATE    = THROW_STOP;         //传球模式关闭
				KICK_STATE     = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;        //遥控模式关闭
				NOW_STATE      = CATCH_4th_BALL;  //当前状态为抓取第4个一类球
			}
		}
		break;
		
			case CATCH_4th_BALL:    //抓取第4个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED)   
			{
				TURN_RESET_STATE = TURN_RESET_STOP; //复位转盘关闭
        ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_STOP;        //夹球模式关闭
				THROW_STATE    = THROW_START;       //传球模式开启
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;      //遥控模式开启
        NOW_STATE      = BALL_4th_TO_PASS_4th;       //当前状态为传第4个一类球
			}
		}
		break;
		
			case BALL_4th_TO_PASS_4th:    //抓取第4个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED)   
			{
				TURN_RESET_STATE = TURN_RESET_STOP; //复位转盘关闭
        ARM_RESET_STATE = ARM_RESET_START;   //复位机械臂开启
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_STOP;        //夹球模式关闭
				THROW_STATE    = THROW_START;       //传球模式开启
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;      //遥控模式开启
        NOW_STATE      = THROW_4th_BALL;       //当前状态为传第4个一类球
			}
		}
		break;
		
		case THROW_4th_BALL:
		{
			// 如果传球成功
			if((THROW_STATE == THROW_SUCCEEDED)&&(ARM_RESET_STATE == ARM_RESET_SUCCEEDED))
			{
				TURN_RESET_STATE = TURN_RESET_START;  //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_START;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_GOING;        //移动模式开启
				CATCH_STATE    = CATCH_STOP;          //夹球模式开启
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
//				ROMOTE_STATE   = ROMOTE_START;        //遥控模式开启
        NOW_STATE      = PASS_4th_TO_START;//当前状态为准备状态，进入遥控模式
			}
		}
		break;
		
		case PASS_4th_TO_START://从夹球点4前往启动点
		{
			// 如果到达启动区
			if(MOVING_STATE == MOVING_ARRIVED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP; //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;   //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_STOP;        //夹球模式关闭
				THROW_STATE    = THROW_STOP;        //传球模式关闭
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;      //遥控模式开启
				NOW_STATE      = START_TO_1st_KICK;//从启动区前往2分区踢球区
			}
		}
		break;
		
		case START_TO_1st_KICK:    //从启动区前往夹球点5（出现接球不中才有这种情况，用遥控选择）
		{
			// 如果到达夹球点5
			if((MOVING_STATE == MOVING_ARRIVED)&&(Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH) == RESET)&&(Read_limit_switch(PULL_RESET_LIMIT_SWITCH) == RESET))
			{
				TURN_RESET_STATE = TURN_RESET_START; //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;   //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_START;       //夹球模式开启
				THROW_STATE    = THROW_STOP;        //传球模式关闭
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;       //遥控模式关闭
        NOW_STATE      = KICK_1st;       //当前状态为抓取第5个一类球
			}
		}
		break;
		
			case KICK_1st:    //当前状态为抓取第5个一类球
		{
	    //如果夹球成功
			if(CATCH_STATE == CATCH_SUCCEEDED)   
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
        ARM_RESET_STATE = ARM_RESET_START;    //复位机械臂开启
				MOVING_STATE    = MOVING_STOP;        //移动模式关闭
				CATCH_STATE     = CATCH_STOP;         //夹球模式关闭
				THROW_STATE     = THROW_START;        //传球模式开启
				KICK_STATE      = KICK_STOP;          //踢球模式关闭
				ROMOTE_STATE    = ROMOTE_START;       //遥控模式开启
        NOW_STATE       = KICK_1st_TO_START;  //当前状态为传第5个一类球
			}
		}
		break;
		
		case KICK_1st_TO_START:
		{
			//如果传球成功
			if((THROW_STATE == THROW_SUCCEEDED)&&(ARM_RESET_STATE == ARM_RESET_SUCCEEDED))
			{
				TURN_RESET_STATE = TURN_RESET_START;  //复位转盘开启
				ARM_RESET_STATE = ARM_RESET_START;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_GOING;        //移动模式开启
				CATCH_STATE    = CATCH_STOP;          //夹球模式开启
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;        //遥控模式关闭
   			NOW_STATE      = START_TO_2nd_KICK;  //当前状态为从夹球点5到踢球区
			}
		}
		break;
		
		case START_TO_2nd_KICK://从夹球点5到踢球区
		{
			// 如果到达启动区
			if(MOVING_STATE == MOVING_ARRIVED)
			{
        TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;   //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;       //移动模式关闭
				CATCH_STATE    = CATCH_STOP;        //夹球模式关闭
				THROW_STATE    = THROW_STOP;        //传球模式关闭
				KICK_STATE     = KICK_STOP;         //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;       //遥控模式关闭
				NOW_STATE      = KICK_2nd;          //当前状态为踢球
			}
		}
		
		case KICK_2nd:
		{
			// 如果到达踢球区2分区
			if(MOVING_STATE == MOVING_ARRIVED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;         //移动模式关闭
//				MOVING_STATE   = MOVING_GOING;       //移动模式开启
				CATCH_STATE    = CATCH_STOP;          //夹球模式关闭
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_START;          //踢球模式开启
				ROMOTE_STATE   = ROMOTE_START;        //遥控模式开启
				NOW_STATE      = TO_BALL_5th;      //当前状态为在2分踢球区踢球
			}
		}
		break;
		
		case TO_BALL_5th:  //在2分踢球区踢球
		{
			// 如果踢球成功
			if(KICK_STATE == KICK_SUCCEEDED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_GOING;        //移动模式开启
				CATCH_STATE    = CATCH_STOP;          //夹球模式关闭
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;         //遥控模式关闭
				NOW_STATE      = CATCH_5th_BALL;  //当前状态为从2分踢球区前往启动区
			}
		}
		break;
		
		case CATCH_5th_BALL: //从2分踢球区前往启动区
		{
			// 如果到达启动区
			if(MOVING_STATE == MOVING_ARRIVED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;         //移动模式关闭
				CATCH_STATE    = CATCH_STOP;          //夹球模式关闭
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_START;        //遥控模式开启
				NOW_STATE      = BALL_5th_TO_PASS_5th;//当前状态为遥控模式
			}
		}
		break;
		
		case BALL_5th_TO_PASS_5th:  //从启动区前往3分踢球区
		{
			// 如果到达3分踢球区
			if(MOVING_STATE == MOVING_ARRIVED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;         //移动模式关闭
				CATCH_STATE    = CATCH_STOP;          //夹球模式关闭
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_START;          //踢球模式开启
				ROMOTE_STATE   = ROMOTE_STOP;         //遥控模式关闭
				NOW_STATE      = THROW_5th_BALL;  //当前状态为在3分踢球区踢球
			}
		}
		break;
		
		case THROW_5th_BALL://在3分踢球区踢球
		{
			// 如果踢球成功
			if(MOVING_STATE == MOVING_ARRIVED)
			{
				TURN_RESET_STATE = TURN_RESET_STOP;   //复位转盘关闭
				ARM_RESET_STATE = ARM_RESET_STOP;     //复位机械臂关闭				
				MOVING_STATE   = MOVING_STOP;         //移动模式关闭
				CATCH_STATE    = CATCH_STOP;          //夹球模式关闭
				THROW_STATE    = THROW_STOP;          //传球模式关闭
				KICK_STATE     = KICK_STOP;           //踢球模式关闭
				ROMOTE_STATE   = ROMOTE_STOP;         //遥控模式关闭
				NOW_STATE      = ROMOTE_PREPARATION;  //结束比赛
			}
		}
		break;
		
		default: break;
	}
	  OSTimeDly(2);
  }
}

