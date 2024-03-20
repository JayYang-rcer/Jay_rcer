#include "includes.h"
#include <app_cfg.h>


int main(void)
{	
	
	OSInit();  //UCOS初始化

	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //创建开始任务
	
	OSStart(); //开始任务
	
	return (0);
}


 


