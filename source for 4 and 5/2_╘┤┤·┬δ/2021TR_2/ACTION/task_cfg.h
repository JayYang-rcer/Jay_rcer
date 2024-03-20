#ifndef __TASKS_H
#define __TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "defines.h"


typedef struct
{	
	float yaw_des;
	float pitch_des;
	int rpm_des;
	u8 is_set;
	
}CTRL_DES_DEF;



#define MAIN_TASK_PRIO		4
#define MAIN_STK_SIZE 		128  
void Main_task(void *pvParameters);


#define PSTWO_TASK_PRIO		2
#define PSTWO_STK_SIZE 		128  
void PStwo_task(void *pvParameters);

#define U3MOTOR_DRIVER_TASK_PRIO		5
#define U3MOTOR_DRIVER_STK_SIZE 		128  
void U3Motor_Driver_task(void *pvParameters);

extern CTRL_DES_DEF CTRL_DES;

void Upper_update_info(CanRxMsg *msg);

#endif
