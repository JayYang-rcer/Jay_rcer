#include "headfiles.h"




void Switch_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 开时钟

	// 初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
}



