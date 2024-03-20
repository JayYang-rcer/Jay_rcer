#ifndef __TASKS_H
#define __TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "defines.h"

extern TaskHandle_t StartTask_Handler;
extern TaskHandle_t RunTimeStats_Handler;
extern TaskHandle_t ChassisDrive_Handler;
extern TaskHandle_t DataScope_Handler;
extern TaskHandle_t Debug_Handler;
extern TaskHandle_t Cast_Handler;
extern TaskHandle_t Move_Handler;
extern TaskHandle_t PStwo_Handler;
extern TaskHandle_t FSM_Handler;


#if USE_DATASCOPE
//DataScope task config
void DataScope_task(void *pvParameters);
#endif

#if USE_DEBUG
//Debug task config
void Debug_task(void *pvParameters);
#endif


// 任务优先级设置
#define DATASCOPE_TASK_PRIO	2
#define DEBUG_TASK_PRIO			3
#define CHASSIS_TASK_PRIO		8
#define CAST_TASK_PRIO			5
#define MOVE_TASK_PRIO			7
#define TAKE_TASK_PRIO			6
#define FSM_TASK_PRIO				9


// 栈大小设置
#define DATASCOPE_STK_SIZE 	128
#define DEBUG_STK_SIZE 			128
#define CHASSIS_STK_SIZE 		128  
#define CAST_STK_SIZE 			128  
#define MOVE_STK_SIZE 			128
#define TAKE_STK_SIZE 			128
#define FSM_STK_SIZE 				128


void ChassisDrive_task(void *pvParameters);
void Cast_task(void *pvParameters);
void Move_task(void *pvParameters);
void Take_task(void *pvParameters);
void FSM_task(void *pvParameters);

#endif
