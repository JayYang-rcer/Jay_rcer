#ifndef __FSM_H
#define __FSM_H
#include "stm32f4xx.h"

// 机器人的主状态
typedef enum ROBOT_STATE_ITEMS
{
	ROMOTE_PREPARATION,                                 //准备状态
	
	START_TO_BALL_1st,                                  //从启动区前往第一个夹球点
	CATCH_1st_BALL,                                 	  //抓取第一个1类球   
	THROW_1st_BALL,                                   	//传第一个1类球
	
	TO_BALL_2nd,                                        //前往第二个夹球点
	CATCH_2nd_BALL,                                     //抓取第二个1类球
	THROW_2nd_BALL,                                     //传第二个1类球
  
	TO_BALL_3rd,                                        //前往第三个夹球点
	CATCH_3rd_BALL,                                     //抓取第三个1类球
	BALL_3st_TO_PASS_3rd,                               //到第三个球传球点
	THROW_3rd_BALL,                                     //传第三1类球
	
	TO_BALL_4th,                                        //前往第四个夹球点
	CATCH_4th_BALL,                                     //抓取第第四个1类球
	BALL_4th_TO_PASS_4th,                               //到第四个球传球点
	THROW_4th_BALL,                                     //传第四个1类球
	
	PASS_4th_TO_START,                                  //从第四个射球点前往启动区
	
	START_TO_1st_KICK,                                    //从启动区前往2分区踢球区
	KICK_1st,                                           //在2分区踢球
	
	KICK_1st_TO_START,                                  //从2分区踢球区前往启动区
	
	START_TO_2nd_KICK,                                    //从启动区前往3分区踢球区
	
  KICK_2nd,                                           //在3分区踢球
	
  TO_BALL_5th,                                        //从踢球点前往第五个夹球点
	CATCH_5th_BALL,                                     //抓取第第五个1类球
	BALL_5th_TO_PASS_5th,                               //到第五个球传球点
	THROW_5th_BALL,                                     //传第五个1类球
	PASS_5th_TO_START,                                  //从第五个夹球点前往启动区
	
}ROBOT_STATE_ITEMS;

// 移动的具体状态
typedef enum MOVING_STATE_ITEMS
{
	MOVING_STOP,
	MOVING_GOING,
	MOVING_ROMOTE,
	MOVING_ARRIVED,
}MOVING_STATE_ITEMS;

// 抛投的具体状态
typedef enum THROW_STATE_ITEMS
{
	THROW_STOP,
	THROW_START,
	THROW_SUCCEEDED,
}THROW_STATE_ITEMS;

//抓球的具体状态
typedef enum CATCH_STATE_ITEMS
{
	CATCH_STOP,
	CATCH_START,
	CATCH_PICK_SUCCEEDED,
	CATCH_SUCCEEDED,
}CATCH_STATE_ITEMS;

//踢球的具体状态
typedef enum KICK_STATE_ITEMS
{
	KICK_STOP,
	KICK_START,
	KICK_SUCCEEDED,
}KICK_STATE_ITEMS;

// 遥控的具体状态
typedef enum ROMOTE_STATE_ITEMS
{
	ROMOTE_STOP,
	ROMOTE_START,
	ROMOTE_OVER,
	ROMOTE_FREE,
}ROMOTE_STATE_ITEMS;

// 准备阶段可选择的模式
typedef enum OPTAINAL_MODE_ITEMS
{
	MODE_UNKNOWN,
	MODE_MOVING_START_TO_CATCH_1st_BALL_POINT,
	MODE_MOVING_CATCH_4th_BALL_POINT_TO_START,
	MODE_ROMOTE_CATCH_4th_BALL_POINT_TO_CATCH_5th_BALL_POINT,
	MODE_MOVING_START_TO_1st_KICK_POINT,	
	MODE_MOVING_START_TO_2nd_KICK_POINT,	
}OPTAINAL_MODE_ITEMS;

typedef enum ARM_RESET_STATE_ITEMS
{
	ARM_RESET_STOP,
	ARM_RESET_START,
	ARM_ARRIVE_SUCCEEDED,
	ARM_RESET_SUCCEEDED,
}ARM_RESET_STATE_ITEMS;

typedef enum TURN_RESET_STATE_ITEMS
{
	TURN_RESET_STOP,
	TURN_RESET_START,
	TURN_RESET_SUCCEEDED,
}TURN_RESET_STATE_ITEMS;


void FSM_RUN(void *pdata);

extern enum ROBOT_STATE_ITEMS NOW_STATE;
extern enum MOVING_STATE_ITEMS MOVING_STATE;
extern enum THROW_STATE_ITEMS THROW_STATE;
extern enum ROMOTE_STATE_ITEMS ROMOTE_STATE;
extern enum OPTAINAL_MODE_ITEMS OPTAINAL_MODE;
extern enum CATCH_STATE_ITEMS CATCH_STATE;
extern enum KICK_STATE_ITEMS KICK_STATE;
extern enum TURN_RESET_STATE_ITEMS TURN_RESET_STATE;
extern enum PULL_RESET_STATE_ITEMS PULL_RESET_STATE;
extern enum ARM_RESET_STATE_ITEMS ARM_RESET_STATE;
extern enum MOVE_ARM_RESET_STATE_ITEMS MOVE_ARM_RESET_STATE;

#endif
